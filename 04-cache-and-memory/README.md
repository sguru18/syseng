# Module 04 -- Cache and Memory

At HFT firms, code is judged not by asymptotic complexity but by cache behavior. An O(n) operation with cache misses loses to an O(n log n) operation that fits in L1. These exercises make you see the hierarchy in real numbers from your own machine.

## Exercises

| # | Exercise | What You Build |
|---|----------|----------------|
| 1 | [Cache Profiler](exercises/ex1-cache-profiler/) | Pointer-chasing latency table + false sharing demo |
| 2 | [Data Layout: AoS vs SoA](exercises/ex2-data-layout/) | Particle benchmark + ARM NEON vectorization |
| 3 | [Hot Path Allocation](exercises/ex3-hot-path-alloc/) | Arena + object pool allocators, P99 tail comparison |

## Build All

```bash
make
make clean
```
