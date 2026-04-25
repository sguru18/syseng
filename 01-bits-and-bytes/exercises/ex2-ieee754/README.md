# Exercise 2: IEEE 754 Float Bit Surgery

## The Idea

A `float` is just 32 bits. Instead of using floating-point instructions, you'll manipulate those bits directly as unsigned integers. This is exactly how GPUs do fast math tricks, how network code converts byte orders, and how ML frameworks implement quantization.

## IEEE 754 Single-Precision Layout

```
 31  30       23  22                    0
┌───┬──────────┬──────────────────────────┐
│ S │ Exponent │        Mantissa          │
│ 1 │  8 bits  │        23 bits           │
└───┴──────────┴──────────────────────────┘
```

- **Sign (S)**: 0 = positive, 1 = negative
- **Exponent**: biased by 127. Stored value 0–255. Actual exponent = stored − 127.
- **Mantissa** (aka fraction): the fractional part after an implicit leading 1 (for normalized numbers).

**The value**: (-1)^S × 1.mantissa × 2^(exponent − 127)

**Special cases**:
- Exponent = 0, Mantissa = 0 → **±Zero**
- Exponent = 0, Mantissa ≠ 0 → **Denormalized** (no implicit leading 1, exponent = −126)
- Exponent = 255, Mantissa = 0 → **±Infinity**
- Exponent = 255, Mantissa ≠ 0 → **NaN**

## Warm-Up

If I give you the raw integer `0x40490FDB`, what float does it represent?

```
0x40490FDB = 0 10000000 10010010000111111011011
             S=0  E=128    Mantissa
             
Exponent = 128 - 127 = 1
Value = 1.10010010000111111011011₂ × 2¹
      = 11.0010010000111111011011₂
      = 3.14159265...
      ≈ π
```

It's pi! The constant `M_PI` stored as a float is `0x40490FDB`.

## Functions to Implement

| Function | What It Does | Key Insight |
|----------|-------------|-------------|
| `float_negate(f)` | Flip the sign | ★ (done for you) |
| `float_abs(f)` | Clear the sign bit | Which single bit controls the sign? |
| `float_twice(f)` | Multiply by 2.0 | What happens to the exponent when you double? What about denormals? |
| `float_half(f)` | Divide by 2.0 | Reverse of twice, but watch the denormal boundary |
| `float_is_nan(f)` | Detect NaN | What makes NaN special in the exponent+mantissa? |
| `int_to_float(x)` | Convert int → float bits | The hardest one. Find the magnitude, normalize, round, and pack the fields. |

## Building & Testing

```bash
make test
```

Tests compare your bit-manipulated results against what the hardware produces using `union { float f; unsigned int u; }` to extract real IEEE 754 bits.

## Real-World Connections

- **Quake III's `Q_rsqrt`**: treats a float as an int, does integer arithmetic on it, then treats the result as a float again. Genius hack, same principle as this exercise.
- **Network protocols**: floats must be serialized in a defined byte order. You can't just `memcpy` a float across architectures.
- **ML quantization**: converting FP32 → FP16 → INT8 requires understanding exactly which bits carry which information.
