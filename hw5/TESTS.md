# HW5

## Build

cmake -S . -B build
cmake --build build

## Build result

[ 25%] Building CXX object CMakeFiles/demo_thread_pool.dir/thread_pool/demo_thread_pool.cpp.o
[ 50%] Linking CXX executable demo_thread_pool
[ 50%] Built target demo_thread_pool
[ 75%] Building CXX object CMakeFiles/demo_process_pool.dir/process_pool/demo_process_pool.cpp.o
[100%] Linking CXX executable demo_process_pool
[100%] Built target demo_process_pool


## ThreadPool demo

## Command

./build/demo_thread_pool

## Output

=== Simple tasks ===
Waiting for future1...
future1 result = 10
Waiting for future2...
future2 result = 20
Waiting for future3...
future3 exception: Task failed

=== More tasks than threads ===
Sum = 150

## ProcessPool demo

##Command

./build/demo_process_pool

## Output

Result 1 = 12
Result 2 = 54
Result 3 = 100
