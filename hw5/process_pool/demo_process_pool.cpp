#include <iostream>
#include <vector>

#include "process_pool.h"

int main() {
    ProcessPool pool(2);

    ProcessTask task1;
    task1.type = ProcessTaskType::Add;
    task1.a = 7;
    task1.b = 5;

    ProcessTask task2;
    task2.type = ProcessTaskType::Multiply;
    task2.a = 6;
    task2.b = 9;

    ProcessTask task3;
    task3.type = ProcessTaskType::SleepAndReturn;
    task3.a = 100;
    task3.delay_ms = 300;

    std::vector<MyFuture<int>> futures;
    futures.push_back(pool.Submit(task1));
    futures.push_back(pool.Submit(task2));
    futures.push_back(pool.Submit(task3));

    std::cout << "Result 1 = " << futures[0].Get() << std::endl;
    std::cout << "Result 2 = " << futures[1].Get() << std::endl;
    std::cout << "Result 3 = " << futures[2].Get() << std::endl;

    return 0;
}
