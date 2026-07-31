# Module 02 — Modern C++ Essentials

Modern C++ isn't just syntactic sugar -- RAII, move semantics, and strong types are the patterns that make low-latency systems correct and fast. These are the C++ idioms HFT firms expect fluency in.

## Exercises

| # | Exercise | Focus |
|---|----------|-------|
| 1 | [RAII and Ownership](exercises/ex1-raii-ownership/) | ScopedFd, UniqueBuffer, ScopeGuard |
| 2 | [Type-Safe Trading Primitives](exercises/ex2-type-safe-primitives/) | Strong typedefs, enum class, constexpr table |
| 3 | [Move Semantics Under the Hood](exercises/ex3-move-semantics/) | Copy vs move, NRVO, perfect forwarding |

## Build

```
make test   # run all exercises
make clean  # remove all binaries
```
