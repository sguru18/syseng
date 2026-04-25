# Exercise 3: Stack Frames

Every time you call a function, the CPU creates a **stack frame**. Every time a function
returns, that frame is destroyed. Debuggers, crash reporters, and profilers all work by
*walking* this chain of frames. In this exercise, you'll do it yourself.

## How the Call Stack Works

When function A calls function B:

1. A pushes the **return address** (the instruction to resume at when B finishes)
2. B saves A's **frame pointer** and sets up its own
3. B allocates space for its local variables
4. When B returns, it restores A's frame pointer and jumps to the return address

The frame pointer creates a **linked list** through the stack. Each frame points to the
previous one. Following this chain gives you a backtrace.

## Stack Layout (ARM64 / Apple Silicon)

```
High addresses (stack grows downward)
┌────────────────────────────┐
│  main()'s frame            │
│  ┌──────────────────────┐  │
│  │ local variables       │  │
│  │ ...                   │  │
│  │ saved x30 (LR)       │  │  ← return address
│  │ saved x29 (FP) ──────│──│──→ points to caller's frame
│  └──────────────────────┘  │
│                            │
├────────────────────────────┤
│  func_a()'s frame          │
│  ┌──────────────────────┐  │
│  │ local variables       │  │
│  │ ...                   │  │
│  │ saved x30 (LR)       │  │  ← return address (back into main)
│  │ saved x29 (FP) ──────│──│──→ points to main's frame
│  └──────────────────────┘  │
│                            │
├────────────────────────────┤
│  func_b()'s frame          │
│  ┌──────────────────────┐  │
│  │ local variables       │  │
│  │ saved x30 (LR)       │  │  ← return address (back into func_a)
│  │ saved x29 (FP) ──────│──│──→ points to func_a's frame
│  └──────────────────────┘  │
│                            │
Low addresses
```

## The ARM64 Frame Pointer Convention

On Apple Silicon (ARM64):
- **x29** (also called FP) is the frame pointer register
- **x30** (also called LR) is the link register (return address)
- At the address pointed to by FP: `*fp` = previous frame's FP
- At fp + 8: `*(fp + 8)` = return address (saved LR)

So to walk the stack:
```
frame_pointer = current FP
while frame_pointer != NULL:
    return_address = *(frame_pointer + 1)   // +1 because pointer arithmetic on void**
    frame_pointer = *frame_pointer          // follow the chain
```

## Warm-Up

What happens when you call a function?

1. The CPU pushes the return address and saves the frame pointer
2. The function does its work
3. The function restores the frame pointer and jumps back

That's it. Every function call. Every time. Billions of times per second on your machine
right now.

## The Exercise

Implement `capture_backtrace()` in `backtrace.c`. It walks the frame pointer chain and
records each frame's pointer and return address.

**Important:** This must be compiled with `-fno-omit-frame-pointer` so the compiler
actually maintains the frame pointer chain. Without this flag, the compiler may skip
saving the frame pointer as an optimization, and your walker will get garbage.

## Real-World Relevance

- **lldb `bt`:** The backtrace command in the debugger does exactly this -- walks the
  frame pointer chain and symbolizes the return addresses.
- **Crash reporters:** When your app crashes, the crash reporter captures a backtrace
  from the faulting thread to include in the crash log.
- **Sampling profilers:** Tools like Instruments and `perf` periodically interrupt the
  program and capture a backtrace to figure out where time is spent.
- **Async stack traces:** Modern runtimes stitch together async stack frames so you can
  see the logical call chain even across await boundaries.

## Build & Test

```bash
make test    # compile and run
make clean   # remove build artifacts
```
