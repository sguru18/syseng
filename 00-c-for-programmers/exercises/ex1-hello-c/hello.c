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
#include <stdbool.h>

int max_of(int a, int b) {
    // TODO: return the larger of a and b
    // (void)a; (void)b;
    return a > b ? a : b;
}

int min_of(int a, int b) {
    // TODO: return the smaller of a and b
    // (void)a; (void)b;
    return a > b ? b : a;
}

int clamp(int val, int lo, int hi) {
    // TODO: if val < lo, return lo. If val > hi, return hi. Otherwise return val.
    // (void)val; (void)lo; (void)hi;
    // return val < lo ? lo : val > hi ? hi : val;

    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;

    // QUESTION: best way to write the above like code quality wise?
}

int array_sum(const int *arr, int n) {
    // TODO: sum all elements arr[0] through arr[n-1] and return the total.
    //
    // In C, `const int *arr` means arr points to ints you can read but not modify.
    // Access elements with arr[i] -- same as C++.

    int sum = 0;
    for(int i = 0; i < n; ++i) {
        sum += arr[i];
    }

    return sum;

    // QUESTION: why is arr passed as *arr. is this a pointer to the first int in the array or something? not by value is so that the array is not copied right
    // yes
}

int array_max_index(const int *arr, int n) {
    // TODO: return the index of the largest element.
    // If n <= 0, return -1.
    //
    // This is like std::max_element but you return the index, not an iterator.

    if ( n<=0 ){
        return -1;
    }
    int largest = arr[0];
    int largest_idx = 0;
    for (int i = 1; i < n; ++i) {
        if (arr[i] > largest) {
            largest = arr[i];
            largest_idx = i;
        }
    }
    return largest_idx;
}

void array_reverse(int *arr, int n) {
    // TODO: reverse arr in place.
    //
    // Swap arr[0] with arr[n-1], arr[1] with arr[n-2], etc.
    // In C there's no std::swap -- just use a temp variable.
    int temp = 0;
    for(int i = 0; i < n / 2; ++i) {
        temp = arr[i];
        int mirror_idx = n - 1 - i;
        arr[i] = arr[mirror_idx];
        arr[mirror_idx] = temp;
    }
}

int array_contains(const int *arr, int n, int val) {
    // TODO: return 1 if val appears in arr, 0 otherwise.
    //
    // C doesn't have `bool` by default (you'd need <stdbool.h>).
    // Convention: functions return int, with 0 = false, nonzero = true.

    if (n <= 0) {
        return 0;
    }

    bool found = false;
    int i = 0;
    while (!found && i < n) {
        if (arr[i++] == val) {
            found = true;
        }
    }

    return found ? 1 : 0;
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
    
    if (n % 15 == 0) {
        sprintf(buf, "FizzBuzz");
    } else if (n % 5 == 0) {
        sprintf(buf, "Buzz");
    } else if (n % 3 == 0) {
        sprintf(buf, "Fizz");
    } else {
        sprintf(buf, "%d", n);
    }
}
