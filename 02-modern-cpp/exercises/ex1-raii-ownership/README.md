# Exercise 1 — RAII and Ownership

RAII = resource lifetime tied to object lifetime. No manual cleanup. This is why `new`/`delete` are banned in production trading code -- use RAII wrappers. A leaked file descriptor in a trading system silently causes reconnect failures at the worst possible moment.

## Parts

1. **ScopedFd** — RAII wrapper for a POSIX file descriptor. Destructor calls `close()` automatically.
2. **UniqueBuffer** — Move-only heap buffer. Build `std::unique_ptr<T[]>` from scratch.
3. **ScopeGuard** — Run a callable on scope exit. The canonical C++ cleanup pattern, used everywhere from kernel code to Chrome.

## By the end of this exercise...

You'll know exactly when destructors run, why copying resource handles is dangerous, and how `std::unique_ptr` works internally. You'll be able to answer the interview question: _"What is RAII and why does it matter for correctness?"_

Answer:

## Build & Test

```
make test
```
