# Exercise 3 — Move Semantics Under the Hood

Every market data message that gets copied unnecessarily is microseconds lost. Move semantics let you transfer ownership in O(1) -- a pointer swap -- instead of O(n) memcpy. HFT code is obsessed with zero-copy paths.

## Parts

1. **Buffer** — A heap-allocated byte buffer with explicit copy and move semantics. Built from scratch so you see every constructor call.
2. **Static counters** — `copy_count` / `move_count` let you instrument exactly when copies happen in a pipeline.
3. **NRVO observation** — Verify that the compiler eliminates copies on function returns.
4. **Perfect-forwarding factory** — `make_buffer` forwards arguments without unnecessary copies.

## By the end of this exercise...

You'll have benchmarked copy vs move on a 1MB buffer and seen the ~1000x difference in real numbers. You'll understand NRVO, when `std::move` is necessary, and why `noexcept` on move constructors matters for standard containers.

## Build & Test

```
make test   # correctness tests + benchmark output
```
