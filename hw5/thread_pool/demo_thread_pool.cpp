#include <chrono>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "thread_pool.h"

using namespace std::chrono_literals;

int main() {
    {
        std::cout << "=== Simple tasks ===" << std::endl;

        ThreadPool pool(3);

        auto future1 = pool.Submit([]() {
            std::this_thread::sleep_for(500ms);
            return 10;
        });

        auto future2 = pool.Submit([]() {
            std::this_thread::sleep_for(300ms);
            return 20;
        });

        auto future3 = pool.Submit([]() {
            std::this_thread::sleep_for(200ms);
            throw std::runtime_error("Task failed");
            return 0;
        });

        std::cout << "Waiting for future1..." << std::endl;
        std::cout << "future1 result = " << future1.Get() << std::endl;

        std::cout << "Waiting for future2..." << std::endl;
        std::cout << "future2 result = " << future2.Get() << std::endl;

        std::cout << "Waiting for future3..." << std::endl;
        try {
            int result = future3.Get();
            std::cout << "future3 result = " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "future3 exception: " << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== More tasks than threads ===" << std::endl;

        ThreadPool pool(2);
        std::vector<MyFuture<int>> futures;

        for (int i = 1; i <= 5; ++i) {
            futures.push_back(pool.Submit([i]() {
                std::this_thread::sleep_for(200ms);
                return i * 10;
            }));
        }

        int sum = 0;
        for (auto& future : futures) {
            sum += future.Get();
        }

        std::cout << "Sum = " << sum << std::endl;
    }

    return 0;
}
