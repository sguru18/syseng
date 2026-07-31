# Exercise 3 – STL-Conformant Pool Allocator

`malloc` has unpredictable tail latency — its internal lock, fragmentation state, and OS interactions can spike to microseconds. A pool allocator eliminates all of that: pre-allocate a flat slab at startup, bump a pointer on each allocation, and never call `free`. This is why every serious HFT library ships its own allocator.

## Real-World Relevance

- **Citadel's custom allocators**: flat pool for order objects, recycled via free-list after cancel
- **Jump Trading message pool**: all order messages are pre-allocated in a contiguous slab at startup

## What You Build

`PoolAllocator<T>` — a C++ Allocator-concept-conformant bump allocator backed by a 4 MB flat array. Drop it into `std::vector<T, PoolAllocator<T>>` and benchmark it against the default allocator.

Key milestones:
1. Implement `allocate(n)` — bump-pointer alloc with 64-byte alignment, throw `std::bad_alloc` on exhaustion
2. Implement `deallocate(p, n)` — no-op for a bump allocator
3. `std::vector<int, PoolAllocator<int>>` works correctly
4. Benchmark shows 2-5× speedup and dramatically lower tail latency (p99, p99.9)

## Running

```bash
make test
```
