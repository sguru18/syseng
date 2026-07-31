# syseng -- Systems Engineering Workbook

Puzzle-driven curriculum targeting quant developer intern roles at HFT firms (Jump, Citadel, Optiver, HRT). Builds from C basics to low-latency C++ systems.

## Quick Start

```bash
cd 02-modern-cpp/exercises/ex1-raii-ownership
cat README.md
make test
```

## Roadmap

### Foundation (C)
| Module | Topic |
|--------|-------|
| [00](00-c-for-programmers/) | C for Programmers -- syntax, strings, malloc, structs |
| [01](01-bits-and-bytes/) | Bits and Bytes -- bitwise ops, IEEE 754, protocol packing |

### Core C++ (the filter)
| Module | Topic | What You Build |
|--------|-------|----------------|
| [02](02-modern-cpp/) | Modern C++ Essentials | RAII, ownership, type-safe order primitives, move semantics |
| [03](03-stl-internals/) | STL Internals | Container benchmarks, custom iterator + ring buffer, STL allocator |

### Hardware Awareness (the differentiator)
| Module | Topic | What You Build |
|--------|-------|----------------|
| [04](04-cache-and-memory/) | Cache and Memory | Measure your own cache hierarchy, AoS vs SoA, arena allocator |
| [05](05-concurrency/) | Concurrency + Memory Model | Seqlock, SPSC ring buffer, MPMC with false sharing |

### Trading Systems
| Module | Topic | What You Build |
|--------|-------|----------------|
| [06](06-order-book/) | Order Book + Matching Engine | Price levels, full LOB with cancel/modify, matching engine |
| [07](07-networking/) | Networking | epoll TCP server, UDP multicast receiver, FIX framer |

## How It Works

Each exercise is a mini-project with connected parts. You start with something broken or naive, build it up, and end with a working system producing real output -- latency numbers from your hardware, a working order book, a network server handling connections. Tests verify correctness; benchmarks show you the numbers.

## Toolchain

- C++17, clang++, make
- `tools/common.h` -- C test macros (used by modules 00-01)
- `tools/common.hpp` -- C++ test macros, timing utilities, benchmark harness

```bash
make test-all    # run everything
make clean-all   # clean build artifacts
```
