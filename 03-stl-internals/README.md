# Module 03 – STL Internals

The STL is your vocabulary — but you have to know the cost of every word. The wrong container in a hot path is a latency disaster. This module makes you think about containers in terms of cache misses, not just API.

## Exercises

| # | Exercise | What You Build |
|---|----------|----------------|
| 1 | [Container Benchmark](exercises/ex1-container-benchmark/) | Empirical insert/lookup/iterate harness across 5 containers |
| 2 | [Circular Buffer + Iterator](exercises/ex2-circular-buffer/) | STL-compatible ring buffer with random-access iterator |
| 3 | [Pool Allocator](exercises/ex3-pool-allocator/) | C++ Allocator-concept-conformant bump allocator |

## Quick Start

```bash
cd exercises/ex1-container-benchmark
cat README.md
make test
```

## Toolchain
- C++17, clang++, ARM64 Mac
- `../../../tools/common.hpp` — test macros + `bench::` timing utilities
