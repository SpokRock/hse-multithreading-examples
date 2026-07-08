# HSE Multithreading Examples

[![CI](https://github.com/SpokRock/hse-multithreading-examples/actions/workflows/ci.yml/badge.svg)](https://github.com/SpokRock/hse-multithreading-examples/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

HSE Multithreading Examples is an educational C++ repository with small, focused examples for learning operating-system processes, C++ threads, synchronization primitives, inter-process communication, futex-based waiting, lock-free or low-locking data structures, and structured concurrency ideas.

The project is intended for reading, running, modifying, and discussing examples. It is **not** a production-ready concurrency library.

## Covered topics

| Topic | Directories | What to look for |
| --- | --- | --- |
| Processes and threads | `processes_and_threads/` | `fork`, `exec`, file descriptor inheritance, POSIX threads, `std::thread`, `std::jthread` |
| Synchronization primitives | `synchronization_primitives/` | mutexes, lock guards, recursive/shared mutexes, spinlocks, barriers, latches, condition variables |
| Atomics and races | `synchronization_primitives/atomics/`, `synchronization_primitives/sync_intro/` | data races, race conditions, compare-and-swap, atomic operations |
| Inter-process communication | `ipc/`, `hw4/` | pipes, FIFOs, signals, semaphores, POSIX message queues, shared memory, mmap, an IPC queue exercise |
| Futex | `futex/`, `tasks/futex.md` | Linux futex-style waiting and wakeup patterns |
| Cache effects | `cache/` | false sharing demonstrations |
| Lock-free / low-locking structures | `lock_free/`, `tasks/synchronization_primitives/*channel/` | single-producer/single-consumer stack and channel tasks |
| Futures and structured concurrency | `structured_concurrency/`, `tasks/future.md` | `std::future`, `std::async`, `std::packaged_task`, shared futures, continuations, optional `stdexec` examples |

See [`docs/topics.md`](docs/topics.md) for a topic map and typical pitfalls.

## Repository structure

```text
.
├── cache/                         # Cache effects and false sharing examples
├── futex/                         # Futex/latch-style synchronization example
├── hw4/                           # Standalone shared-memory IPC queue exercise
├── include/                       # Small shared utility headers
├── ipc/                           # Inter-process communication examples
├── lock_free/                     # Lock-free / low-locking examples
├── processes_and_threads/         # fork, exec, pthread, and C++ thread examples
├── structured_concurrency/        # future/async/continuation examples
├── synchronization_primitives/    # mutex, condvar, barrier, latch, atomics examples
├── tasks/                         # Exercise statements and task-oriented examples
└── docs/                          # Project overview, build notes, and topic guide
```

## Requirements

The main example set is built with CMake and a recent C++ compiler.

- Linux is recommended. Several examples use POSIX or Linux-specific APIs such as `fork`, `exec`, POSIX shared memory, message queues, semaphores, and futex-related behavior.
- CMake 3.25 or newer for the root project.
- A C++23-capable compiler such as recent GCC or Clang.
- POSIX threads, discovered by CMake as `Threads::Threads`.
- Network access is needed only when building optional examples or standalone tasks that fetch third-party dependencies with CMake `FetchContent`.

## Build the examples

Configure and build the default target set:

```bash
cmake -S . -B build -DHSE_BUILD_STDEXEC=OFF
cmake --build build -j
```

`HSE_BUILD_STDEXEC` is disabled in the command above so the common build does not download `stdexec`. To build the examples that depend on NVIDIA `stdexec`, configure with:

```bash
cmake -S . -B build-stdexec -DHSE_BUILD_STDEXEC=ON
cmake --build build-stdexec -j
```

## Run examples

After building, run a target from the build directory. Examples are intentionally small and independent:

```bash
./build/processes_and_threads/stl/stl_intro
./build/synchronization_primitives/mutex/lock_guard
./build/ipc/pipe_intro
./build/lock_free/spsc_stack
```

Some programs demonstrate incorrect or dangerous patterns, such as data races or deadlocks. Read the source before running unknown targets, especially targets with names such as `deadlock`, `data_race`, or `*_tsan`.

## Tests and benchmarks

The root project currently uses compilation as the primary smoke check because many examples are interactive, timing-sensitive, intentionally racy, or demonstrate blocking behavior.

Standalone task directories contain GoogleTest and Google Benchmark based checks:

```bash
cmake -S tasks/synchronization_primitives/buffered_channel -B build-buffered-channel
cmake --build build-buffered-channel -j
./build-buffered-channel/buffered_channel_test
./build-buffered-channel/buffered_channel_benchmark

cmake -S tasks/synchronization_primitives/unbuffered_channel -B build-unbuffered-channel
cmake --build build-unbuffered-channel -j
./build-unbuffered-channel/unbuffered_channel_test
./build-unbuffered-channel/unbuffered_channel_benchmark
```

The [`hw4/`](hw4/) IPC queue exercise is a separate CMake project; see [`hw4/TESTS.md`](hw4/TESTS.md) for a sample build and run transcript.

## Learning goals

This repository is useful for learners who want to:

- compare processes, POSIX threads, and C++ standard threads;
- understand common synchronization primitives and when each primitive is appropriate;
- observe race conditions, data races, deadlocks, livelocks, and starvation risks;
- practice reasoning about memory ordering and atomic compare-and-swap loops;
- learn basic POSIX IPC mechanisms and shared-memory synchronization;
- experiment with futures, continuations, and structured-concurrency style APIs;
- use sanitizers and focused examples to investigate concurrency bugs.

## Project limitations

- The examples are educational and intentionally compact.
- The repository is not a reusable production library.
- Some examples intentionally contain problematic concurrency patterns to make bugs observable.
- Linux/POSIX behavior is assumed for many process and IPC examples.
- Performance numbers from local benchmarks are machine-specific and should not be treated as project-wide metrics.

## Open-source status

This is a public educational repository maintained for studying concurrency concepts. Corrections, documentation improvements, portability fixes, and careful educational examples are welcome. The project does not claim production readiness, external adoption, download counts, coverage percentages, or security-score metrics.

## Contributing

Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) before opening a pull request. In short:

- keep examples focused and easy to inspect;
- do not hide the concurrency concept behind unnecessary framework code;
- document how to build and run new examples;
- avoid adding large dependencies unless they are essential for a specific educational purpose.

## Roadmap

Planned improvements are intentionally modest and education-focused:

- add short notes for more existing examples;
- expand smoke checks where examples are deterministic and safe to run in CI;
- document sanitizer usage for selected racy examples;
- add more diagrams for IPC and synchronization flows;
- improve portability notes for non-Linux environments.

## License

The original code and documentation in this repository are licensed under the [MIT License](LICENSE), unless a file explicitly states otherwise. If external course materials or third-party snippets are added later, they should keep their original copyright and license notices.
