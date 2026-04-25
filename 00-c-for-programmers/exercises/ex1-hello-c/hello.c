/*
 * Hello C -- your first C file.
 *
 * Fill in each TODO. These are small functions to get you
 * comfortable with C syntax, printf, arrays, and basic logic.
 *
 * Key differences from C++ to keep in mind:
 *   - No references. Use pointers: void foo(int *x) { *x = 5; }
 *   - Arrays don't know their own length. You always pass the size too.
 *   - Strings are char arrays ending with '\0'. Use sprintf() to write into them.
 */

#include "hello.h"
#include <stdio.h>    // printf, sprintf
#include <string.h>   // strlen, strcmp, strcpy

int max_of(int a, int b) {
    // TODO: return the larger of a and b
    (void)a; (void)b;
    return 0;
}

int min_of(int a, int b) {
    // TODO: return the smaller of a and b
    (void)a; (void)b;
    return 0;
}

int clamp(int val, int lo, int hi) {
    // TODO: if val < lo, return lo. If val > hi, return hi. Otherwise return val.
    (void)val; (void)lo; (void)hi;
    return 0;
}

int array_sum(const int *arr, int n) {
    // TODO: sum all elements arr[0] through arr[n-1] and return the total.
    //
    // In C, `const int *arr` means arr points to ints you can read but not modify.
    // Access elements with arr[i] -- same as C++.
    (void)arr; (void)n;
    return 0;
}

int array_max_index(const int *arr, int n) {
    // TODO: return the index of the largest element.
    // If n <= 0, return -1.
    //
    // This is like std::max_element but you return the index, not an iterator.
    (void)arr; (void)n;
    return -1;
}

void array_reverse(int *arr, int n) {
    // TODO: reverse arr in place.
    //
    // Swap arr[0] with arr[n-1], arr[1] with arr[n-2], etc.
    // In C there's no std::swap -- just use a temp variable.
    (void)arr;
    (void)n;
}

int array_contains(const int *arr, int n, int val) {
    // TODO: return 1 if val appears in arr, 0 otherwise.
    //
    // C doesn't have `bool` by default (you'd need <stdbool.h>).
    // Convention: functions return int, with 0 = false, nonzero = true.
    (void)arr;
    (void)n;
    (void)val;
    return 0;
}

void fizzbuzz(int n, char *buf) {
    // TODO: write the fizzbuzz result for n into buf.
    //
    // Use sprintf(buf, "format", args...) to write into a char buffer.
    // It works like printf but writes to a string instead of stdout.
    //
    // Examples:
    //   sprintf(buf, "Fizz");        -- writes "Fizz\0" into buf
    //   sprintf(buf, "%d", 42);      -- writes "42\0" into buf
    //   sprintf(buf, "hello %d", n); -- writes "hello 42\0" into buf
    (void)n;
    (void)buf;
}
