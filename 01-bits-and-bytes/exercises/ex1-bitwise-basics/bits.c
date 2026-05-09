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
    // wow took me much longer than i thought it would but figured it out
    // pretty cool
    // thought is that x >> 31 is either 11111111 or 00000000 
    // XOR with x gives you positive version but if it was neg it's off by 1
    // but 11111111 = -1 so -11111111 adds 1 and -00000000 doesn't change obvi
    // don't fully understand right shift yet though
    return (x ^ (x >> 31)) - (x >> 31);
    // matthew's idea something like this
    // return (~x & (x >> 31)) | (x & ~(x >> 31));
}

/* TODO: Return 1 if x is a positive power of 2 (1, 2, 4, 8, ...), else 0. */
int is_power_of_2(int x) {
    // x >> 31 gives the mask 00000000 or 11111111
    // if it is a power of two then binary will be 000000010000 all 0s except a 1
    // how to count the number of 1s 
    // there is a function later to count number of one bits
    // oh wait we can clear the lowest set bit which has to make it all 0s
    // so (x & (x-1)) should be 00000000 if true but we want 1 
    // so then plus 1?
    // this doesn't get 0 for a negative number though
    // or what about minus the negative
    // that would make a negative 0 and still add 1 here
    // return (x & (x-1)) - (~x + 1);
    // wait negative of what 
    // 0100 -> 0000 - (~0000 >> 31) = 0 + 1 = 1 correct
    // 1010 -> 1000 - (~1000 >> 31) = 1000 - 0000 = 1000 er nvm

    // ((x & (x-1)) - ~(x & (x-1))) - ((x & (x-1)) - ~(x & (x-1)) >> 31);
    // holy moly wrote out a lot of stuff and tried different combos of operations 
    // to try to match expected output values
    // last step would not be necessary if i could output 1 / -1 
    // actually that accepts negative numbers like -7
    // oh lmao i messed up the formula
    // need to use bitabs
    // can i use variables?

    // finally used claude for help after third session still not yielding progress
    // very helpful iea to split into clauses like this
    int v = x & (x-1);
    int not_zero = (x | (~x+1)) >> 31;  // -1 if x!=0, 0 if x==0
    int not_neg = ~(x >> 31);            // -1 if x>=0, 0 if x<0
    int v_zero = ~((v | (~v+1)) >> 31); // -1 if v==0, 0 if v!=0
    return v_zero & not_zero & not_neg & 1;
}

/* TODO: Return 1 if x + y does NOT overflow (signed 32-bit), else 0. */
int add_ok(int x, int y) {
    return 0;
}

/* TODO: Return 1 if x is negative, else 0. */
int is_negative(int x) {
    // think we should get the mask which is 0 for pos or -1 for neg
    // negative of that is 0 for pos or 1 for neg nice
    return ~(x >> 31) + 1;
}

/* TODO: If x is nonzero, return y; otherwise return z. No branching! */
int conditional(int x, int y, int z) {
    return 0;
}

/* TODO: Return the number of 1-bits in x (population count). */
int bit_count(int x) {
    // can be used to solve power of 2
    // the mask is 00000000 for 0_______ or 11111111 for 1_______
    // 
}
