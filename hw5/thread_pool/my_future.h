#pragma once

#include <memory>
#include <utility>

#include "shared_state.h"

template <typename T>
class MyFuture {
public:
    explicit MyFuture(std::shared_ptr<SharedState<T>> state)
        : state_(std::move(state)) {
    }

    MyFuture(const MyFuture&) = delete;
    MyFuture& operator=(const MyFuture&) = delete;

    MyFuture(MyFuture&&) = default;
    MyFuture& operator=(MyFuture&&) = default;

    void Wait() {
        state_->Wait();
    }

    T Get() {
        return state_->Get();
    }

    bool IsReady() {
        return state_->IsReady();
    }

private:
    std::shared_ptr<SharedState<T>> state_;
};
