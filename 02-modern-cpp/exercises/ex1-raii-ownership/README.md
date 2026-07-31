# Exercise 1 — RAII and Ownership

RAII = resource lifetime tied to object lifetime. No manual cleanup. This is why `new`/`delete` are banned in production trading code -- use RAII wrappers. A leaked file descriptor in a trading system silently causes reconnect failures at the worst possible moment.

## Parts

1. **ScopedFd** — RAII wrapper for a POSIX file descriptor. Destructor calls `close()` automatically.
2. **UniqueBuffer** — Move-only heap buffer. Build `std::unique_ptr<T[]>` from scratch.
3. **ScopeGuard** — Run a callable on scope exit. The canonical C++ cleanup pattern, used everywhere from kernel code to Chrome.

## By the end of this exercise...

You'll know exactly when destructors run, why copying resource handles is dangerous, and how `std::unique_ptr` works internally. You'll be able to answer the interview question: _"What is RAII and why does it matter for correctness?"_

Answer: RAII stands for resource acquisition is initialization, which is a resource management paradigm that couples a stack object's lifetime to the lifetime of a resource it possesses (ie. memory). The main benefit of this is the guarantee that once the stack object's lifetime has ended (gone out of scope, stack unwind, etc.), any resources it posessed have also been properly cleaned up. This prevents myriad bugs like double free resulting from improper owner cleanup. Practically, it means you'll never really write plain new/delete pairs in your own code outside of RAII wrapper classes.

## Build & Test

```
make test
```
