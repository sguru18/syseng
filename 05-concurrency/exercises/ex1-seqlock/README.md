# Exercise 1 — Sequence Lock

The seqlock is used in Linux's time subsystem (`gettimeofday`) and in many trading systems for publishing market data snapshots. It allows concurrent readers at zero cost when no writer is active — the common case. Writers are exclusive but don't block readers.

## Files

- `seqlock.hpp` — implement `SeqLock<T>::write()` and `SeqLock<T>::read()`
- `test_seqlock.cpp` — correctness tests + throughput benchmark vs `std::shared_mutex`

## Build & run

```
make
./test_seqlock
```
