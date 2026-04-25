# Exercise 2: Strings and Arrays

## C Strings: The Single Biggest Difference

In C++, `std::string` handles everything for you -- allocation, resizing, copying, comparison. In C, a string is just a `char` array with a `\0` (null byte) at the end. That's it. No length field. No methods.

```c
char greeting[] = "hello";  // compiler makes this: {'h','e','l','l','o','\0'}
                             // sizeof(greeting) == 6 (5 chars + null terminator)
```

The null terminator `\0` is how every C string function knows where the string ends. Forget it and your program will read random memory until it happens to hit a zero byte. This is the source of approximately 40% of all C bugs.

## Key String Functions (from `<string.h>`)

```c
size_t strlen(const char *s);           // length (NOT including \0)
int    strcmp(const char *a, const char *b);  // 0 if equal, <0 if a<b, >0 if a>b
char  *strcpy(char *dst, const char *src);   // copy src into dst (dst must be big enough!)
char  *strcat(char *dst, const char *src);   // append src to end of dst
char  *strncpy(char *dst, const char *src, size_t n);  // copy at most n bytes
```

The dangerous pattern: `strcpy(dst, src)` doesn't check if `dst` is big enough. Buffer overflow. This is how heartbleed-style vulnerabilities happen.

## malloc / free: Manual Memory

In C++ you have `new` and `delete` (and smart pointers). In C:

```c
#include <stdlib.h>

int *arr = malloc(10 * sizeof(int));  // allocate 10 ints on the heap
if (arr == NULL) {
    // malloc returns NULL on failure -- always check!
}
arr[0] = 42;  // use it like a normal array
free(arr);     // YOU must free it. No RAII. No garbage collector.
arr = NULL;    // good habit: null the pointer after free
```

Key rules:
- Every `malloc` needs exactly one `free`. Miss it = memory leak. Double free = crash.
- `malloc` returns `void *`. In C (unlike C++), `void *` implicitly converts to any pointer type -- no cast needed.
- `sizeof` is your friend: `malloc(n * sizeof(int))` not `malloc(n * 4)`. Sizes vary by platform.

## Pointer Arithmetic

Arrays and pointers are deeply connected in C:

```c
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;       // array name decays to pointer to first element
printf("%d\n", *p);      // 10  (dereference)
printf("%d\n", *(p+1));  // 20  (pointer + 1 moves by sizeof(int) bytes)
printf("%d\n", p[2]);    // 30  (p[i] is syntactic sugar for *(p+i))
```

`p + 1` doesn't add 1 byte -- it adds `sizeof(*p)` bytes. The compiler knows the type and scales automatically.

## The Exercise

Implement string and dynamic array functions in `strings.c`. You'll practice:
- Working with null-terminated strings manually
- Allocating and freeing heap memory
- Pointer arithmetic
- The `const` keyword for read-only access

## Building & Testing

```bash
make test
```

## Think About It

After finishing, ask yourself: why doesn't C just store the string length alongside the data? Some systems do exactly that (Pascal strings, Rust's `String`, Go's `string`). Redis's SDS (Simple Dynamic Strings) stores the length before the char data. There's a trade-off: C strings are simple, zero-overhead, and universally compatible -- but every `strlen` call is O(n).
