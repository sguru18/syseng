# Exercise 3 — MPMC Queue and False Sharing

MPMC queues are harder than SPSC — you need CAS to claim slots. But the more interesting lesson is false sharing: putting the head and tail pointer on the same cache line can destroy performance even in the SPSC case. This exercise shows you the number.

## Files

- `mpmc_queue.hpp` — implement `MPMCQueue<T,N>` and false-sharing benchmark helpers
- `test_mpmc_queue.cpp` — correctness (4P/4C) + MPMC vs SPSC-like throughput + false-sharing speedup

## Build & run

```
make
./test_mpmc_queue
```
