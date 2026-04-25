/*
 * Bitwise Basics
 *
 * Rules: You may ONLY use the following operators:
 *   ~  &  |  ^  <<  >>
 *
 * You may also use + (addition) and integer constants.
 *
 * You may NOT use:
 *   - if, else, switch, ternary (?:)
 *   - for, while, do-while
 *   - Comparison operators: <, >, <=, >=, ==, !=
 *   - Multiplication, division, modulo (*, /, %)
 *   - Casting to other types
 *
 * Assume 32-bit int with two's complement representation.
 * Assume arithmetic right shift (sign bit fills on >>).
 */

#include "bits.h"

/*
 * Example: return x & y
 * This one is done for you to show the expected style.
 */
int bit_and(int x, int y) {
    return x & y;
}

/*
 * TODO: Return -x (the negation of x) without using the unary minus operator.
 * Recall how two's complement negation works.
 */
int bit_negate(int x) {
    return 0;
}

/*
 * TODO: Return the absolute value of x.
 * Think about how the sign bit can create a mask, and how negation works.
 */
int bit_abs(int x) {
    return 0;
}

/*
 * TODO: Return 1 if x is a positive power of 2 (1, 2, 4, 8, ...), else 0.
 * Powers of 2 have a special property in binary. What does x & (x-1) tell you?
 * Be careful: 0 is not a power of 2, and neither are negative numbers.
 */
int is_power_of_2(int x) {
    return 0;
}

/*
 * TODO: Return 1 if x + y does NOT overflow (for signed 32-bit ints), else 0.
 * Overflow happens when two positive numbers sum to negative, or two negatives sum to positive.
 * Can you detect this by looking at the sign bits of x, y, and x+y?
 */
int add_ok(int x, int y) {
    return 0;
}

/*
 * TODO: Return 1 if x is negative, else 0.
 * What does the most significant bit tell you?
 */
int is_negative(int x) {
    return 0;
}

/*
 * TODO: If x is nonzero, return y; otherwise return z.
 * No if/else or ternary allowed! You need a bitmask that is either all-1s or all-0s
 * depending on whether x is zero or not.
 */
int conditional(int x, int y, int z) {
    return 0;
}

/*
 * TODO: Return the number of 1-bits (population count / Hamming weight) in x.
 * The naive approach examines each bit individually (32 iterations), but you can't use loops.
 * Consider a divide-and-conquer approach: count pairs, then nibbles, then bytes, then halves.
 */
int bit_count(int x) {
    return 0;
}
