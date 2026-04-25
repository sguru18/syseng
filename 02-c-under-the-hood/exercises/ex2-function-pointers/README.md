# Exercise 2: Function Pointers

Data pointers hold addresses of data. Function pointers hold addresses of *code*.
Same idea, different segment of memory. That's it.

## Reading the Syntax

Function pointer syntax looks alien until you learn to read it inside-out:

```c
void (*fp)(int)
```

Read it like this:
1. `fp` -- the variable name
2. `*fp` -- fp is a pointer
3. `(*fp)(int)` -- to a function that takes an `int`
4. `void (*fp)(int)` -- and returns `void`

A `typedef` makes it cleaner:

```c
typedef void (*callback_fn)(int);
callback_fn fp;  // same thing, much cleaner
```

## The Exercise

You'll build a **generic sort function** -- one that can sort *any* data type, as long
as you give it a comparator function. This is the exact pattern behind `qsort()` in libc.

The trick: everything is `void*`. The sort function doesn't know what it's sorting.
It just shuffles bytes around and asks the comparator "which one comes first?"

**You implement:**
1. `generic_sort()` -- a sorting algorithm that works with `void*` arrays
2. Four comparator functions for different data types and orderings

The swap helper is provided. The test harness is provided. You fill in the brain.

## How `void*` Generic Programming Works

```c
// To access element i in a void* array:
char *base = (char *)arr;
void *elem_i = base + i * elem_size;

// Inside a comparator for int arrays:
int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);  // -1, 0, or 1
}
```

## Real-World Relevance

- **libc `qsort()`:** The C standard library's sort uses this exact function pointer
  pattern. You're reimplementing it.
- **C++ vtables:** Virtual method dispatch is a table of function pointers. When you
  call `obj->method()`, the compiler looks up the function pointer in the vtable.
- **Plugin architectures:** Shared libraries export function pointers. The host app
  calls them without knowing the implementation at compile time.
- **Signal handlers:** `signal(SIGINT, handler_fn)` registers a function pointer the
  OS calls when the signal fires.
- **Callbacks everywhere:** Event loops, GUI frameworks, async I/O -- all function
  pointers (or closures, which are function pointers + captured state).

## Build & Test

```bash
make test    # compile and run
make clean   # remove build artifacts
```
