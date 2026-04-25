# Exercise 1: Hello C

## What This Is

Your first C file. The goal isn't to learn anything deep -- it's to get your hands on the compiler, see how `printf` works, and establish that C is just a simpler version of what you already know.

## The C You Already Know

If you've written C++, you already know 80% of C's syntax:
- `int`, `char`, `float`, `double` -- same
- `if`, `else`, `for`, `while`, `switch` -- same
- `+`, `-`, `*`, `/`, `%`, `&&`, `||`, `!` -- same
- Arrays: `int arr[10]` -- same
- Functions: `int add(int a, int b) { return a + b; }` -- same

## The printf Cheat Sheet

C doesn't have `cout`. You use `printf` with format specifiers:

```c
int x = 42;
float f = 3.14;
char c = 'A';
char *s = "hello";

printf("int: %d\n", x);       // %d for int
printf("float: %f\n", f);     // %f for float
printf("char: %c\n", c);      // %c for single char
printf("string: %s\n", s);    // %s for char*
printf("hex: 0x%x\n", x);     // %x for hex
printf("pointer: %p\n", &x);  // %p for pointer address
printf("size: %zu\n", sizeof(x));  // %zu for size_t
```

`\n` is the newline. `printf` does NOT add one automatically (unlike `cout << endl`).

## What You Need to Know: Header Files

In C++ you might `#include <iostream>`. In C:
- `#include <stdio.h>` -- for `printf`, `scanf`, `FILE*`
- `#include <stdlib.h>` -- for `malloc`, `free`, `atoi`, `exit`
- `#include <string.h>` -- for `strlen`, `strcmp`, `memcpy`, `memset`
- `#include <stdbool.h>` -- for `bool`, `true`, `false`
- `#include <stdint.h>` -- for `int32_t`, `uint8_t`, etc.

## The Exercise

Open `hello.c` and fill in the TODO functions. They're all small -- this is about getting comfortable, not solving hard problems.

## Building & Testing

```bash
make test
```

## Warm-Up: Compile By Hand

Before using the Makefile, try compiling manually to see what's happening:

```bash
clang -std=c11 -Wall -o hello hello.c
./hello
```

`-Wall` turns on all warnings. Get used to running with it -- warnings in C often mean bugs.
