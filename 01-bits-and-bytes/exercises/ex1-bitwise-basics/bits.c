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
 *
 * INCLUDES: Figure out what you need to include.
 */

/* TODO: add your #include lines here */
#include <stdlib.h>

/*
 * Example: return x & y
 * This one is done for you to show the expected style.
 */
int bit_and(int x, int y) {
    return x & y;
}

/* TODO: Return -x without using the unary minus operator. */
int bit_negate(int x) {
    // what the flip
    // so all 1s represents -1
    // 0 is 00000000... so ~0 flips these into all 1s which is -1
    // so ~x = (2^n - 1) which is all 1s and then - x bc flipping everything
    // and then ~x + 1 = 2^n - 1 - x + 1 = 2^n - x
    // and 2^n - x = -x because numbers are stored mod 2^n (when it wraps around) so 2^n is actually 0
    // wow this is cool  
    return ~x + 1;
}

/* TODO: Return the absolute value of x. */
int bit_abs(int x) {
    return 0;
}

/* TODO: Return 1 if x is a positive power of 2 (1, 2, 4, 8, ...), else 0. */
int is_power_of_2(int x) {
    return 0;
}

/* TODO: Return 1 if x + y does NOT overflow (signed 32-bit), else 0. */
int add_ok(int x, int y) {
    return 0;
}

/* TODO: Return 1 if x is negative, else 0. */
int is_negative(int x) {
    return 0;
}

/* TODO: If x is nonzero, return y; otherwise return z. No branching! */
int conditional(int x, int y, int z) {
    return 0;
}

/* TODO: Return the number of 1-bits in x (population count). */
int bit_count(int x) {
    return 0;
}
