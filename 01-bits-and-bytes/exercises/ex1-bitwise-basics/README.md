# Exercise 1: Bitwise Basics

## The Challenge

Implement 8 functions using **only** these operators: `~  &  |  ^  <<  >>`

That's it. No `if`, no `else`, no ternary `?:`, no loops, no comparisons (`<`, `>`, `==`), no multiplication, no division, no casting to other types. Just raw bit manipulation.

This constraint is the puzzle. It forces you to think about what these operators *actually do* to 32 bits of data.

## Warm-Up: Build Your Intuition

Before diving in, grab a scrap of paper and work through these:

**What does `x & (x - 1)` do?**
Try it:
- `x = 8`  → `1000 & 0111` = `0000`
- `x = 12` → `1100 & 1011` = `1000`
- `x = 7`  → `0111 & 0110` = `0110`

See the pattern? It clears the lowest set bit. This is one of the most useful bit tricks in existence.

**What is `-1` in binary (two's complement)?**
All 1s: `1111...1111`. So `~0 == -1`. And `~x + 1 == -x`. Two's complement is your friend throughout this exercise.

**What does `x >> 31` give you (for a 32-bit int)?**
If `x` is negative, arithmetic right shift fills with 1s → result is `-1` (all 1s).
If `x` is non-negative → result is `0` (all 0s).
This is a branchless way to create a mask from a sign bit.

## Functions to Implement

| Function | Description | Difficulty |
|----------|-------------|------------|
| `bit_and(x, y)` | Return `x & y` | ★ (done for you) |
| `bit_negate(x)` | Return `-x` without using `-` | ★★ |
| `bit_abs(x)` | Return absolute value of `x` | ★★ |
| `is_negative(x)` | Return 1 if `x < 0`, else 0 | ★ |
| `is_power_of_2(x)` | Return 1 if `x` is a positive power of 2 | ★★ |
| `add_ok(x, y)` | Return 1 if `x + y` doesn't overflow | ★★★ |
| `conditional(x, y, z)` | Return `y` if `x != 0`, else `z` (no branching!) | ★★★ |
| `bit_count(x)` | Count the number of 1-bits in `x` | ★★★ |

## Building & Testing

```bash
make test
```

All tests will fail initially — that's expected. Implement the functions in `bits.c` and watch them turn green one by one.

## Hints

- Two's complement negation: `-x == ~x + 1`. But wait, you can't use `+`... or can you? (Think about what `~x + 1` means, and whether the rules say anything about `+`.)
- For `conditional`: you need a mask that's either all-1s or all-0s based on whether `x` is zero. How do you turn any nonzero value into all-1s?
- For `bit_count`: the brute-force approach needs 32 shifts. Can you do it in O(log n) steps by counting in parallel? Look up "sideways addition" or "population count."
