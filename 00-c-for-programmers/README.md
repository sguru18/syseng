# Module 00: C for Programmers

> You know how to code. You've written C++. You've just never written *C*.
> This module gets you fluent in 3 exercises.

## C is Not C++ Minus Features

It's tempting to think of C as "C++ with stuff removed." That framing misses the point. C is a different language with a different philosophy: **you are the machine's translator.** There is no runtime doing work behind your back -- no constructors, no destructors, no hidden copies, no vtable dispatch, no exceptions unwinding the stack.

Everything that happens, you wrote. That's terrifying and liberating in equal measure.

## What's Different From C++ (The Short Version)

| In C++ you have... | In C you do... |
|---------------------|----------------|
| `std::string` | `char[]` with a `\0` at the end |
| `std::vector` | `malloc` + manual size tracking |
| `class` with methods | `struct` + standalone functions that take a pointer to it |
| `new` / `delete` | `malloc()` / `free()` |
| References (`int &x`) | Pointers (`int *x`) -- always |
| `bool` | `#include <stdbool.h>`, or just use `int` (0 = false, nonzero = true) |
| Templates | `void *` + function pointers (you'll see this in Module 02) |
| Exceptions | Return codes, `errno`, or just crash |
| RAII | Discipline (or forget and leak memory -- the OS will tell you) |
| Namespaces | Prefixed names (`dns_pack`, `bump_alloc`) |
| `std::cout << x` | `printf("%d\n", x)` |
| Overloaded functions | Different names (`print_int`, `print_str`) |

## The Compilation Model

In C++, you might be used to `g++ main.cpp -o main`. C works the same way under the hood, but it's worth being explicit about the pipeline because you'll need it for debugging:

```
source.c  →  preprocessor (#include, #define)  →  compiler  →  object file (.o)  →  linker  →  executable
```

- **Preprocessor**: copy-pastes `#include` files, expands `#define` macros. This is *textual* -- no type checking.
- **Compiler**: turns each `.c` file into a `.o` independently. Each `.c` file is its own world -- it can't see other `.c` files.
- **Linker**: glues `.o` files together, resolves function calls across files.

Headers (`.h`) are just promises: "this function exists somewhere." The `.c` file provides the actual code.

## Exercises

| # | Exercise | What You'll Learn |
|---|----------|-------------------|
| 1 | **Hello C** | Compilation, printf, basic types, arrays, control flow -- your first working C programs |
| 2 | **Strings and Arrays** | C strings, null terminators, manual memory with malloc/free, pointer arithmetic |
| 3 | **Structs and Pointers** | Structs as data bags, passing by pointer, typedef patterns, the `->` operator |

## Building & Testing

```bash
make test    # run all exercise tests
make clean   # clean all build artifacts
```

## The Only Rule

Don't use any C++ features. No `//` comments in headers (use `/* */`), no `bool` without `<stdbool.h>`, no `new`/`delete`. If it doesn't compile with `clang -std=c11`, it's not C.

(Okay, `//` comments are actually fine in C11. But the mindset matters.)
