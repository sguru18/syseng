# Exercise 2 -- Data Layout: AoS vs SoA

AoS = natural for OOP. SoA = natural for hardware. When you process all X coordinates, SoA gives contiguous memory and enables SIMD. AoS scatters them across structs. HFT market data processing, ML inference, and game physics all switch to SoA for exactly this reason.

## Parts

**Part 1 -- Array of Structs (AoS).** Each particle is a struct `{x, y, z, vx, vy, vz, mass}`. Processing only positions loads the full 28-byte struct but uses 12 bytes. Cache lines are wasted on fields you never read.

**Part 2 -- Struct of Arrays (SoA).** All `x` values are contiguous, all `y` values contiguous. Processing positions loads only `x`, `y`, `z` arrays -- zero waste. Four consecutive `x` values fit in one `vld1q_f32` load.

**Part 3 -- ARM NEON SIMD.** Compute 4 distances per loop iteration using `float32x4_t`. The speedup compounds: SoA gives better cache utilization AND enables vectorization that AoS cannot exploit.

## Expected output (Apple M-series)

```
Version             ns/particle      Speedup
-----------         -----------      -------
AoS                        ~4.2            -
SoA                        ~2.0         2.1x
SoA+SIMD                   ~0.7         6.0x
```

## Build

```bash
make
./data_layout
```
