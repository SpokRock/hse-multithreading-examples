#pragma once

#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "my_future.h"
#include "shared_state.h"

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count = std::thread::hardware_concurrency())
        : stop_(false) {
        if (thread_count == 0) {
            thread_count = 4;
        }

        for (size_t i = 0; i < thread_count; ++i) {
            workers_.emplace_back([this]() {
                WorkerLoop();
            });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }

        cv_.notify_all();

        for (auto& worker : workers_) {
            worker.join();
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <typename Func>
    auto Submit(Func func) -> MyFuture<std::invoke_result_t<Func>> {
        using ResultType = std::invoke_result_t<Func>;

        auto state = std::make_shared<SharedState<ResultType>>();

        std::function<void()> task = [func = std::move(func), state]() mutable {
            try {
                auto result = func();
                state->SetValue(std::move(result));
            } catch (...) {
                state->SetException(std::current_exception());
            }
        };

        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (stop_) {
                throw std::runtime_error("ThreadPool is stopped");
            }

            tasks_.push(std::move(task));
        }

        cv_.notify_one();

        return MyFuture<ResultType>(state);
    }

private:
    void WorkerLoop() {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(mutex_);

                cv_.wait(lock, [this]() {
                    return stop_ || !tasks_.empty();
                });

                if (stop_ && tasks_.empty()) {
                    return;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            task();
        }
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;
};
