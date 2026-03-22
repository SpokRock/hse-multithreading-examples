cmake_minimum_required(VERSION 3.25)

project(hw5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(demo_thread_pool thread_pool/demo_thread_pool.cpp)
target_link_libraries(demo_thread_pool PRIVATE Threads::Threads)
target_include_directories(demo_thread_pool PRIVATE thread_pool)

add_executable(demo_process_pool process_pool/demo_process_pool.cpp)
target_include_directories(demo_process_pool PRIVATE process_pool)
