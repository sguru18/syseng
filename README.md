# syseng -- Systems Engineering Workbook

A puzzle-driven curriculum that builds from bits to ML systems. No lectures. The code teaches.

## How It Works

Each module is a self-contained set of exercises. You get scaffolded code with the boring parts done (Makefiles, test harnesses, I/O) -- you fill in the functions that matter. Every exercise has:

- A brief that tells you **what** to build and **why** it matters, but not how
- A scaffold with `TODO` markers where you write the core logic
- An auto-grader: run `make test` and get instant pass/fail feedback

The exercises are designed as puzzles. They're meant to be satisfying to solve, not draining to grind through.

## Quick Start

New to C? Start with Module 00:
```bash
cd 00-c-for-programmers/exercises/ex1-hello-c
cat README.md        # read the brief
# edit hello.c       # fill in the TODOs
make test            # see how you did
```

Already comfortable with C? Jump straight to Module 01:
```bash
cd 01-bits-and-bytes/exercises/ex1-bitwise-basics
cat README.md        # read the brief
# edit bits.c        # fill in the TODOs
make test            # see how you did
```

Run all tests for a module:
```bash
cd 01-bits-and-bytes
make test
```

Run everything:
```bash
make test-all
```

## Roadmap

### Phase 0: Getting Started

| Module | Topic | What You Build |
|--------|-------|----------------|
| **00** | [C for Programmers](00-c-for-programmers/) | printf, strings, malloc/free, structs, linked lists -- C fundamentals for someone who already codes |

### Phase 1: The Machine

| Module | Topic | What You Build |
|--------|-------|----------------|
| **01** | [Bits and Bytes](01-bits-and-bytes/) | Bitwise puzzles, IEEE 754 float manipulation, DNS header packing |
| **02** | [C Under the Hood](02-c-under-the-hood/) | Struct layout predictions, generic sort with function pointers, stack frame walker |
| **03** | [Memory Arena](03-memory-arena/) | Bump allocator, free-list malloc, pool allocator |

### Phase 2: Concurrency and OS

| Module | Topic | What You Build |
|--------|-------|----------------|
| **04** | [Data Structures, Raw](04-data-structures-raw/) | Hash map, lock-free stack, mmap'd B-tree |
| **05** | [Processes and Signals](05-processes-and-signals/) | Mini-shell, process watchdog, process jail |
| **06** | [Threading and Sync](06-threading-and-sync/) | DIY mutex, ring buffer producer-consumer, mini MapReduce |
| **07** | [Virtual Memory](07-virtual-memory/) | Page table simulator, copy-on-write fork, page replacement |
| **08** | [File Systems](08-file-systems/) | FAT filesystem, journaling, FUSE mount |

### Phase 3: Networking

| Module | Topic | What You Build |
|--------|-------|----------------|
| **09** | [Networking](09-networking/) | TCP over UDP, HTTP server with event loop, RPC framework |

### Phase 4: ML Systems

| Module | Topic | What You Build |
|--------|-------|----------------|
| **10** | [ML Systems](10-ml-systems-fundamentals/) | Tensor library, autograd engine, GEMM optimization |

Modules 00-03 are fully built out. The rest are coming -- or you can ask an agent to build them when you're ready.

## Structure

```
syseng/
├── README.md              <- you are here
├── Makefile               <- `make test-all` runs everything
├── tools/                 <- shared test framework
│   ├── check.py           <- test runner with colors and timing
│   └── common.h           <- C test macros (ASSERT_EQ, RUN_TEST, etc.)
├── 00-c-for-programmers/  <- Module 00 (ready -- start here if new to C)
├── 01-bits-and-bytes/     <- Module 01 (ready)
├── 02-c-under-the-hood/   <- Module 02 (ready)
├── 03-memory-arena/       <- Module 03 (ready)
├── 04-data-structures-raw/   <- placeholder
├── ...
└── 10-ml-systems-fundamentals/  <- placeholder
```

Each exercise lives in its own directory with its own README, Makefile, scaffold, and tests. No cross-module dependencies. Want to redo or replace a module? Delete the folder, ask an agent to rebuild it, done.

## Toolchain

- **C compiler**: clang (comes with Xcode Command Line Tools)
- **Build**: make
- **Debugger**: lldb
- **No external dependencies** for Phase 1-3

## Design Principles

**Intuition first.** Every exercise starts with a warm-up that builds the key insight before you touch code. The "aha moment" comes from reasoning, not from being told the answer.

**Real-world connections.** Every exercise explains where the pattern shows up in production systems. You're not doing toy problems -- you're building simplified versions of real infrastructure.

**Modular and replaceable.** Don't like how an exercise is structured? Each one is self-contained. Point an agent at the specific exercise directory and describe what you want changed.

**Fun over rigor.** These are puzzles, not problem sets. The constraint *is* the game (bitwise-only operations, void* generics, embedded free lists). The tests tell you if you won.
