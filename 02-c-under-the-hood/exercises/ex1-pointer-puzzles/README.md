# Exercise 1: Pointer Puzzles

Can you predict how C lays out structs in memory -- *before* running the code?

## The Rules of the Game

C compilers don't just pack struct fields back to back. They insert **padding** to
satisfy **alignment requirements**. The rules are simple:

1. **Each type wants its address to be divisible by its size.**
   - `char` (1 byte) can go anywhere
   - `short` (2 bytes) wants even addresses
   - `int` (4 bytes) wants addresses divisible by 4
   - `double` (8 bytes) wants addresses divisible by 8
   - Pointers (8 bytes on 64-bit) want addresses divisible by 8

2. **The struct's overall alignment = its most-aligned member.**
   - A struct with a `double` inside has 8-byte alignment
   - The struct's total size is rounded up to a multiple of this alignment

3. **Fields are laid out in declaration order.** The compiler never reorders them.

## Warm-Up

What is `sizeof(struct { char a; int b; })`?

It's **not 5**. Here's why:

```
Offset  0: [a ] (char, 1 byte)
Offset  1: [pad] [pad] [pad]     ← 3 bytes padding so b lands on a 4-byte boundary
Offset  4: [b       b       b       b ] (int, 4 bytes)
Total: 8 bytes (already a multiple of 4, no trailing padding needed)
```

## The Exercise

Open `puzzles.c`. You'll see functions like `predict_sizeof_simple()` that return `-1`.

**Your job:** replace each `-1` with your prediction of the actual `sizeof` or `offsetof`
value. Then run `make test`. The tests compare your predictions against reality.

Getting them wrong is the point -- that's how you build the mental model.

## Memory Layout Examples

### struct simple { char a; int b; char c; }

```
Offset:  0       1  2  3    4  5  6  7    8       9  10 11
       +----+---+---+---+----+----+----+----+----+---+---+---+
       | a  | . | . | . | b  | b  | b  | b  | c  | . | . | . |
       +----+---+---+---+----+----+----+----+----+---+---+---+
        char  pad pad pad        int           char  pad pad pad
                                                     (trailing)
sizeof = 12     (. = padding)
```

Why trailing padding after `c`? The struct's alignment is 4 (from `int b`), so total
size must be a multiple of 4. 9 rounds up to 12.

### struct packed_attempt { char a; char b; int c; }

```
Offset:  0     1     2  3    4  5  6  7
       +----+----+---+---+----+----+----+----+
       | a  | b  | . | . | c  | c  | c  | c  |
       +----+----+---+---+----+----+----+----+
        char char pad pad        int
sizeof = 8
```

Same fields, different order, **4 bytes smaller**. Field ordering matters.

## Real-World Relevance

- **Performance:** Misaligned accesses can be 2-10x slower. Cache-line-aware struct
  layout is critical in game engines and HPC code.
- **Network protocols:** On-the-wire formats use `__attribute__((packed))` to avoid
  padding, but then you need to handle alignment manually on the receiving end.
- **GPU programming:** Uniform buffer objects in Vulkan/Metal have strict alignment
  rules (std140/std430). Get them wrong and your shaders read garbage.
- **Security:** Padding bytes are uninitialized. Leaking them to userspace is a real
  kernel vulnerability class (CVE-level bugs).

## Build & Test

```bash
make test    # compile and run
make clean   # remove build artifacts
```
