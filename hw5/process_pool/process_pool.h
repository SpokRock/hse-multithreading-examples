#pragma once

#include <cstddef>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "../thread_pool/my_future.h"
#include "process_task.h"

class ProcessPool {
public:
    explicit ProcessPool(size_t process_count = 2) {
        if (process_count == 0) {
            process_count = 2;
        }

        for (size_t i = 0; i < process_count; ++i) {
            StartWorker();
        }
    }

    ~ProcessPool() {
        {
            std::lock_guard<std::mutex> lock(submit_mutex_);
            stop_ = true;
        }

        for (auto& worker : workers_) {
            ProcessTask task;
            task.type = ProcessTaskType::Shutdown;

            WriteAll(worker.write_fd, &task, sizeof(task));
            close(worker.write_fd);
        }

        for (auto& worker : workers_) {
            waitpid(worker.pid, nullptr, 0);
        }

        for (auto& worker : workers_) {
            if (worker.reader.joinable()) {
                worker.reader.join();
            }
        }
    }

    ProcessPool(const ProcessPool&) = delete;
    ProcessPool& operator=(const ProcessPool&) = delete;

    MyFuture<int> Submit(ProcessTask task) {
        auto state = std::make_shared<SharedState<int>>();
        size_t worker_index = 0;

        {
            std::lock_guard<std::mutex> lock(submit_mutex_);

            if (stop_) {
                throw std::runtime_error("ProcessPool is stopped");
            }

            task.task_id = next_task_id_++;
            worker_index = next_worker_;
            next_worker_ = (next_worker_ + 1) % workers_.size();

            {
                std::lock_guard<std::mutex> state_lock(states_mutex_);
                states_[task.task_id] = state;
            }

            if (!WriteAll(workers_[worker_index].write_fd, &task, sizeof(task))) {
                std::lock_guard<std::mutex> state_lock(states_mutex_);
                states_.erase(task.task_id);
                throw std::runtime_error("Failed to send task to worker process");
            }
        }

        return MyFuture<int>(state);
    }

private:
    struct WorkerInfo {
        pid_t pid = -1;
        int write_fd = -1;
        std::thread reader;
    };

    void StartWorker() {
        int parent_to_child[2];
        int child_to_parent[2];

        if (pipe(parent_to_child) == -1) {
            throw std::runtime_error("pipe(parent_to_child) failed");
        }

        if (pipe(child_to_parent) == -1) {
            close(parent_to_child[0]);
            close(parent_to_child[1]);
            throw std::runtime_error("pipe(child_to_parent) failed");
        }

        pid_t pid = fork();
        if (pid < 0) {
            close(parent_to_child[0]);
            close(parent_to_child[1]);
            close(child_to_parent[0]);
            close(child_to_parent[1]);
            throw std::runtime_error("fork failed");
        }

        if (pid == 0) {
            close(parent_to_child[1]);
            close(child_to_parent[0]);

            ChildLoop(parent_to_child[0], child_to_parent[1]);
            _exit(0);
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        WorkerInfo worker;
        worker.pid = pid;
        worker.write_fd = parent_to_child[1];
        worker.reader = std::thread([this, read_fd = child_to_parent[0]]() {
            ReaderLoop(read_fd);
        });

        workers_.push_back(std::move(worker));
    }

    static void ChildLoop(int read_fd, int write_fd) {
        while (true) {
            ProcessTask task;

            if (!ReadAll(read_fd, &task, sizeof(task))) {
                break;
            }

            if (task.type == ProcessTaskType::Shutdown) {
                break;
            }

            ProcessResult result = ExecuteProcessTask(task);

            if (!WriteAll(write_fd, &result, sizeof(result))) {
                break;
            }
        }

        close(read_fd);
        close(write_fd);
    }

    void ReaderLoop(int read_fd) {
        while (true) {
            ProcessResult result;

            if (!ReadAll(read_fd, &result, sizeof(result))) {
                break;
            }

            std::shared_ptr<SharedState<int>> state;

            {
                std::lock_guard<std::mutex> lock(states_mutex_);

                auto it = states_.find(result.task_id);
                if (it == states_.end()) {
                    continue;
                }

                state = it->second;
                states_.erase(it);
            }

            if (result.success) {
                state->SetValue(result.value);
            } else {
                state->SetException(
                    std::make_exception_ptr(std::runtime_error(result.error))
                );
            }
        }

        close(read_fd);
    }

    static bool WriteAll(int fd, const void* data, size_t size) {
        const char* buffer = static_cast<const char*>(data);
        size_t written = 0;

        while (written < size) {
            ssize_t result = write(fd, buffer + written, size - written);
            if (result <= 0) {
                return false;
            }
            written += static_cast<size_t>(result);
        }

        return true;
    }

    static bool ReadAll(int fd, void* data, size_t size) {
        char* buffer = static_cast<char*>(data);
        size_t read_bytes = 0;

        while (read_bytes < size) {
            ssize_t result = read(fd, buffer + read_bytes, size - read_bytes);
            if (result <= 0) {
                return false;
            }
            read_bytes += static_cast<size_t>(result);
        }

        return true;
    }

private:
    std::vector<WorkerInfo> workers_;

    std::mutex submit_mutex_;
    std::mutex states_mutex_;

    std::unordered_map<int, std::shared_ptr<SharedState<int>>> states_;

    int next_task_id_ = 1;
    size_t next_worker_ = 0;
    bool stop_ = false;
};
