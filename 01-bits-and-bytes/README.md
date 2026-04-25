# Module 01: Bits and Bytes

> "All data is just bits. Types are merely the lens through which we view them."

## Why Bits Matter

Every system you'll ever build rests on a foundation of bits. This isn't academic trivia — it's how real systems work:

- **Unix file permissions** are bit flags: `rwxr-xr-x` = `0755` = `111 101 101`
- **TCP headers** pack flags (SYN, ACK, FIN) into individual bits to keep packets small
- **GPU shaders** treat floats as raw integers for fast approximate math ([Quake's fast inverse square root](https://en.wikipedia.org/wiki/Fast_inverse_square_root))
- **SIMD instructions** operate on 128/256/512-bit vectors by treating them as packed lanes
- **Network protocols** demand exact bit layouts that compiler struct padding would silently corrupt

If you can't think in bits, you can't debug a corrupted packet, write a device driver, or understand why `0.1 + 0.2 != 0.3`.

## Prerequisites

- Basic C: variables, functions, pointers, `printf` (Module 00 covers this if you're new to C)
- A terminal with `clang` (or `gcc`) and `make`

## Exercises

| # | Exercise | What You'll Do |
|---|----------|----------------|
| 1 | **Bitwise Basics** | Implement classic bit manipulation functions using *only* bitwise operators — no `if`, no loops, no comparisons |
| 2 | **IEEE 754** | Tear apart floating-point numbers by operating directly on their 32-bit representations |
| 3 | **Bitfield Packing** | Manually pack and unpack a real DNS header into network byte order — the same way every protocol parser works |

## Building & Testing

From this directory:

```bash
make test    # run all exercise tests
make clean   # clean all build artifacts
```

Or enter any exercise directory and run `make test` individually.

## Key Insight

A `float`, an `int`, and a `char[4]` can all occupy the same 32 bits of memory. The bits don't change — only your interpretation does. A pointer is just a number. A string is just bytes. A color is just four packed integers. Once you internalize this, systems programming stops being magic and starts being engineering.
