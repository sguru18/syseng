#ifndef HELLO_H
#define HELLO_H

#include <stddef.h>

/* Return the larger of a and b */
int max_of(int a, int b);

/* Return the smaller of a and b */
int min_of(int a, int b);

/* Clamp val to be within [lo, hi] */
int clamp(int val, int lo, int hi);

/* Return the sum of all elements in arr (length n) */
int array_sum(const int *arr, int n);

/* Return the index of the maximum element in arr (length n).
 * If n <= 0, return -1. */
int array_max_index(const int *arr, int n);

/* Reverse the array in-place */
void array_reverse(int *arr, int n);

/* Return 1 if val is found in arr (length n), else 0 */
int array_contains(const int *arr, int n, int val);

/* FizzBuzz: write the result for number n into buf.
 * - If divisible by both 3 and 5: "FizzBuzz"
 * - If divisible by 3: "Fizz"
 * - If divisible by 5: "Buzz"
 * - Otherwise: the number as a string (e.g., "7")
 * buf is guaranteed to be large enough. */
void fizzbuzz(int n, char *buf);

#endif /* HELLO_H */
