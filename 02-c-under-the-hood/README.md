# Module 02: C Under the Hood

C isn't magic -- it's a thin layer over the machine. This module peels that layer back.

Every pointer you've ever used is just a number -- an address in memory. Every struct
is just a recipe telling the compiler how to interpret the bytes starting at that address.
Once you internalize this, you stop memorizing rules and start *seeing* the machine.

Understanding pointers, memory layout, and calling conventions doesn't just make you
better at C. It unlocks debugging superpowers: you'll read disassembly, make sense of
crash dumps, and reason about performance at a level most engineers never reach.

## Exercises

| # | Exercise | What You'll Build |
|---|----------|-------------------|
| 1 | **Pointer Puzzles** | Predict struct sizes, field offsets, and alignment -- then verify with code |
| 2 | **Function Pointers** | Build a generic, type-agnostic sort using comparator callbacks |
| 3 | **Stack Frames** | Walk the call stack by following frame pointers -- how debuggers work |

## Key Insight

> A pointer is just a number.
> A struct is just a recipe for interpreting bytes at that number.

That's it. Everything else -- arrays, strings, vtables, even function calls -- is built
on top of these two ideas. Once you see it, you can't unsee it.

## Getting Started

```bash
make test    # run all exercises
make clean   # clean all build artifacts
```

Or work on exercises individually:

```bash
cd exercises/ex1-pointer-puzzles && make test
```
