# Build and test notes

## Root examples

```bash
cmake -S . -B build -DHSE_BUILD_STDEXEC=OFF
cmake --build build -j
```

This validates that the default educational examples compile. The default build avoids downloading `stdexec`.

## Optional stdexec examples

```bash
cmake -S . -B build-stdexec -DHSE_BUILD_STDEXEC=ON
cmake --build build-stdexec -j
```

Use this only when network access is available and you want to inspect the continuation examples that depend on `stdexec`.

## Standalone channel tasks

```bash
cmake -S tasks/synchronization_primitives/buffered_channel -B build-buffered-channel
cmake --build build-buffered-channel -j
./build-buffered-channel/buffered_channel_test

cmake -S tasks/synchronization_primitives/unbuffered_channel -B build-unbuffered-channel
cmake --build build-unbuffered-channel -j
./build-unbuffered-channel/unbuffered_channel_test
```

Benchmarks are available in the same build directories as `buffered_channel_benchmark` and `unbuffered_channel_benchmark`.
