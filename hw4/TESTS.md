# HW4

## Build

mkdir -p build
cd build
cmake ..
cmake --build .

## Build result

[ 12%] Building CXX object CMakeFiles/producer_hw4.dir/producer_hw4.cpp.o
[ 25%] Building CXX object CMakeFiles/producer_hw4.dir/ipc_queue.cpp.o
[ 37%] Building CXX object CMakeFiles/producer_hw4.dir/shm_utils.cpp.o
[ 50%] Linking CXX executable producer_hw4
[ 50%] Built target producer_hw4
[ 62%] Building CXX object CMakeFiles/consumer_hw4.dir/consumer_hw4.cpp.o
[ 75%] Building CXX object CMakeFiles/consumer_hw4.dir/ipc_queue.cpp.o
[ 87%] Building CXX object CMakeFiles/consumer_hw4.dir/shm_utils.cpp.o
[100%] Linking CXX executable consumer_hw4
[100%] Built target consumer_hw4


## Producer run

## Command * 3:

./producer_hw4 /hw4_queue_test 5000

## Output:

hared memory opened successfully
Pushed NUMBER message: 12345
Pushed TEXT message: hello from producer
Queue size: 5000 bytes


## Consumer run

./consumer_hw4 /hw4_queue_test 5000

## Output:

Skipped message with another type
Received TEXT message: hello from producer
Skipped message with another type
Received TEXT message: hello from producer
Skipped message with another type
Received TEXT message: hello from producer
Shared memory opened successfully
Queue is valid
Received TEXT messages: 3
Skipped messages: 3
Queue size: 5000 bytes
Shared memory cleaned up




