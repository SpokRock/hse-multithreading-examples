# Project overview

This repository collects compact C++ examples for lectures, self-study, and code review sessions about multithreading and concurrent programming.

## Design principles

- Keep each example small enough to read in one sitting.
- Prefer explicit standard-library or POSIX primitives over helper frameworks.
- Preserve intentionally flawed examples when they demonstrate a concurrency bug.
- Document build and run commands close to the examples that need special handling.

## Example lifecycle

A good educational example should answer four questions:

1. What concept does it demonstrate?
2. How can a learner build and run it?
3. What output or behavior should the learner inspect?
4. What can go wrong if the primitive is used incorrectly?
