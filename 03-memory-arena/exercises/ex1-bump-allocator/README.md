# Exercise 1: Bump Allocator

## The Idea

Imagine a stack of sticky notes. You write on the next blank one. You never erase a single note -- when you're done with the whole stack, you throw it all away and start fresh.

That's a bump allocator. You have a buffer, a pointer that starts at the beginning, and each allocation "bumps" the pointer forward. When you're done, you reset the pointer back to zero. Done.

It sounds too simple to be useful. It's actually one of the most important allocation strategies in systems programming.

## Why Is This Useful?

Many allocations share the same lifetime:

- **Web server**: all memory for a single request. Allocate headers, body, response buffers -- then free everything when the response is sent.
- **Game engine**: all memory for a single frame. Allocate transforms, draw calls, particle data -- then free everything when the frame is rendered.
- **Compiler**: all memory for a single compilation pass. LLVM's `BumpPtrAllocator` powers most of its intermediate representation.

The bump allocator is **perfect** for these patterns. O(1) allocation (just pointer arithmetic), zero metadata overhead, and "freeing" is a single pointer reset.

## Warm-Up

Before you code anything, think through this:

> You have a 1024-byte buffer. You've allocated 100 bytes so far, so the bump pointer is at offset 100.
>
> **Q1:** Where does the next 32-byte allocation go? What's the new offset?
>
> **Q2:** Now you're at offset 132. The next allocation is 8 bytes but requires **8-byte alignment**. Offset 132 is not a multiple of 8. Where does this allocation actually go? (Hint: the next multiple of 8 after 132 is 136.)
>
> **Q3:** What's the general formula to round up `x` to the next multiple of `n`, where `n` is a power of 2?

That last question is the crux of this exercise.

## Your Task

Open `bump.c`. The glue code is done -- you just need to implement `bump_alloc()`.

The function takes a size and an alignment, and returns a pointer to the allocated region (or NULL if there's not enough space). The key challenge is **alignment**: you need to round up the current offset so the returned pointer is properly aligned.

**The alignment formula** (for power-of-two `align`):
```
aligned = (offset + align - 1) & ~(align - 1)
```

Why does this work? `~(align - 1)` creates a mask that zeroes out the low bits. For `align = 8`, `align - 1 = 7 = 0b111`, so `~7 = ...11111000`. AND-ing with this mask rounds down to the nearest multiple of 8. Adding `align - 1` first ensures we round *up* instead of down.

## Files

| File | What's in it |
|------|-------------|
| `bump.h` | Data structure and function declarations |
| `bump.c` | Implementation -- **fill in `bump_alloc()`** |
| `test_bump.c` | Tests (run with `make test`) |

## Build & Test

```bash
make test    # compile and run tests
make clean   # remove build artifacts
```

All tests will fail until you implement `bump_alloc`. One at a time, make them pass.

## Hints

- The alignment formula is given above. Use it.
- Don't forget to check that the aligned offset + size doesn't exceed capacity.
- `bump_alloc_default` is already implemented for you -- it just calls `bump_alloc` with align=8.
- If you're confused, add `printf` statements to see what offset values you're computing.
