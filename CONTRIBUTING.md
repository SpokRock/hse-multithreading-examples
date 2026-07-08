# Contributing

Contributions are welcome when they keep the repository useful as an educational concurrency project.

## Good contributions

- Fix a build error, typo, undefined behavior that is not part of an intentional demonstration, or unclear explanation.
- Improve documentation for an existing example.
- Add a focused example for a concurrency concept that is not already covered.
- Add a deterministic smoke test for an example that is safe to run in CI.

## Expectations for examples

- Keep examples small and readable.
- Prefer direct use of C++ standard-library, POSIX, or Linux primitives.
- Do not add framework-like wrappers that hide the concept being taught.
- Document special runtime requirements, expected blocking behavior, or intentionally unsafe behavior.
- Avoid large dependencies unless the example specifically teaches that dependency.

## Development workflow

```bash
cmake -S . -B build -DHSE_BUILD_STDEXEC=OFF
cmake --build build -j
```

If you change standalone channel tasks, also build and run their local tests as described in `docs/build-and-test.md`.

## Pull requests

Please include:

- a short explanation of the concept or problem addressed;
- build and test commands that were run;
- any known limitations or follow-up work.
