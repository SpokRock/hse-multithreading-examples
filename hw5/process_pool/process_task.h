#pragma once

#include <chrono>
#include <cstring>
#include <thread>

enum class ProcessTaskType {
    Shutdown,
    Add,
    Multiply,
    SleepAndReturn
};

struct ProcessTask {
    int task_id = 0;
    ProcessTaskType type = ProcessTaskType::Add;
    int a = 0;
    int b = 0;
    int delay_ms = 0;
};

struct ProcessResult {
    int task_id = 0;
    bool success = true;
    int value = 0;
    char error[128] = {};
};

inline ProcessResult ExecuteProcessTask(const ProcessTask& task) {
    ProcessResult result;
    result.task_id = task.task_id;

    try {
        switch (task.type) {
            case ProcessTaskType::Shutdown:
                result.value = 0;
                break;

            case ProcessTaskType::Add:
                result.value = task.a + task.b;
                break;

            case ProcessTaskType::Multiply:
                result.value = task.a * task.b;
                break;

            case ProcessTaskType::SleepAndReturn:
                std::this_thread::sleep_for(std::chrono::milliseconds(task.delay_ms));
                result.value = task.a;
                break;
        }
    } catch (...) {
        result.success = false;
        std::strncpy(result.error, "Task execution failed", sizeof(result.error) - 1);
    }

    return result;
}
