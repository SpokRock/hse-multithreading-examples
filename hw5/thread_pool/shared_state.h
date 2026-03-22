#pragma once

#include <condition_variable>
#include <exception>
#include <mutex>
#include <optional>
#include <utility>

template <typename T>
class SharedState {
public:
    void SetValue(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = std::move(value);
        ready_ = true;
        cv_.notify_all();
    }

    void SetException(std::exception_ptr exception) {
        std::lock_guard<std::mutex> lock(mutex_);
        exception_ = exception;
        ready_ = true;
        cv_.notify_all();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return ready_; });
    }

    T Get() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return ready_; });

        if (exception_) {
            std::rethrow_exception(exception_);
        }

        return std::move(*value_);
    }

    bool IsReady() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ready_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    bool ready_ = false;
    std::optional<T> value_;
    std::exception_ptr exception_;
};
