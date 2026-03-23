# hw2

## BufferedChannel

Команда запуска тестов:

.\build-buffered\Debug\buffered_channel_test.exe

Результат:

[==========] Running 6 tests from 1 test suite.
[ RUN      ] Correctness.Simple
[       OK ] Correctness.Simple
[ RUN      ] Correctness.Senders
[       OK ] Correctness.Senders
[ RUN      ] Correctness.Receivers
[       OK ] Correctness.Receivers
[ RUN      ] Correctness.SmallBuf
[       OK ] Correctness.SmallBuf
[ RUN      ] Correctness.BigBuf
[       OK ] Correctness.BigBuf
[ RUN      ] Correctness.Random
[       OK ] Correctness.Random

[  PASSED  ] 6 tests.


Команда запуска benchmark:

.\build-buffered\Debug\buffered_channel_benchmark.exe

Результат:

2026-03-23T17:20:15+03:00
Running C:\Users\asus\VSCodeProjects\hse-multithreading-examples\build-buffered\Debug\buffered_channel_benchmark.exe
Run on (8 X 2803 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 1280 KiB (x4)
  L3 Unified 12288 KiB (x1)
***WARNING*** Library was built as DEBUG. Timings may be affected.
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
Run/2/2/6/min_time:0.100/process_time/real_time            1553 ms         7062 ms            1
Run/10/4/4/min_time:0.100/process_time/real_time            836 ms         4016 ms            1
Run/100000/4/4/min_time:0.100/process_time/real_time        428 ms         2797 ms            1


## UnbufferedChannel

Команда запуска тестов:

.\build-unbuffered\Debug\unbuffered_channel_test.exe

Результат:

[==========] Running 6 tests from 2 test suites.

[ RUN      ] Correctness.Simple
[       OK ] Correctness.Simple
[ RUN      ] Correctness.Senders
[       OK ] Correctness.Senders
[ RUN      ] Correctness.Receivers
[       OK ] Correctness.Receivers
[ RUN      ] Correctness.BigBuf
[       OK ] Correctness.BigBuf

[ RUN      ] Block.Sender
[       OK ] Block.Sender
[ RUN      ] Block.Receiver
[       OK ] Block.Receiver

[  PASSED  ] 6 tests.