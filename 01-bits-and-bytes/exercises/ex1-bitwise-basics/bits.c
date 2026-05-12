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

    // finally used claude for help after third sitting
    // ohhhh very helpful idea to split into clauses 
    int v = x & (x-1);
    int not_zero = (x | (~x+1)) >> 31;  // -1 if x!=0, 0 if x==0
    int not_neg = ~(x >> 31);            // -1 if x>=0, 0 if x<0
    int v_zero = ~((v | (~v+1)) >> 31); // -1 if v==0, 0 if v!=0
    return v_zero & not_zero & not_neg & 1;
}

/* TODO: Return 1 if x is negative, else 0. */
int is_negative(int x) {
    // think we should get the mask which is 0 for pos or -1 for neg
    // negative of that is 0 for pos or 1 for neg nice
    return ~(x >> 31) + 1;
}

/* TODO: Return 1 if x + y does NOT overflow (signed 32-bit), else 0. */
int add_ok(int x, int y) {
    // claude to help recognize that this must mean inputs have same sign 
    // AND output has opposite sign
    return ~(is_negative(x) ^ is_negative(y)) & ~(is_negative(x) ^ is_negative(x+y)) & 1;
    // close but not quite, progress
}

/* TODO: If x is nonzero, return y; otherwise return z. No branching! */
int conditional(int x, int y, int z) {
    // from is power of 2
    int non_zero = (x | (~x+1)) >> 31; // 0 iff x == 0, else -1
    // if non_zero, return y can be achieved by | y because 0 | y = y
    // if zero, return z can be achieved by 1111 suppose z = 0101 & right yes keeps only set bits from z
    // so have to make both use & or both use |
    // 0000 | 0101 = 0101
    // 1111 & 1001 = 1001
    //
    // 0000 ^ 0101 = 0101 good
    // 1111 ^ 1001 = 0110 bad

    // wait a minute no branching means need to use y and z in the expression regardless 
    // non_zero & y, non_zero & z always 0 if x is 0
    // so non_zero & y + non_zero & z + z gives z correctly

    // non_zero & y = y, non_zero & z = z if it is non_zero
    // so non_zero & y + non_zero & z - z gives y

    // these expressions are very similar to each other
    // oh what if i use the negated mask

    // so non_zero & y + non_zero & z + (~non_zero & z) gives z correctly if zero
    // and non_zero & y + non_zero & z + (~non_zero & z) gives y + z this isn't it

    // what about non_zero & y + non_zero & z + (~non_zero & z) - non_zero & z = y + z - 0 - z = y
    // and if 0 then non_zero & y + non_zero & z + (~non_zero & z) - non_zero & z = 0 + 0 + z - 0 = z
    // no way

    return (non_zero & y) + (non_zero & z) + ((~non_zero) & z) - (non_zero & z);
    // yippee
}

/* TODO: Return the number of 1-bits in x (population count). */
int bit_count(int x) {
    // can be used to solve power of 2
    // the mask is 00000000 for 0_______ or 11111111 for 1_______
    // i dont need the mask here do i
    // says naive needs 32 shifts
    // just check if rightmost bit is 1 and sum
    return (x & 1) + ((x >> 1) & 1) + ((x >> 2) & 1) + ((x >> 3) & 1) + ((x >> 4) & 1) + ((x >> 5) & 1) + ((x >> 6) & 1) + ((x >> 7) & 1) +
    ((x >> 8) & 1) + ((x >> 9) & 1) + ((x >> 10) & 1) + ((x >> 11) & 1) + ((x >> 12) & 1) + ((x >> 13) & 1) + ((x >> 14) & 1) + ((x >> 15) & 1) + 
    ((x >> 16) & 1) + ((x >> 17) & 1) + ((x >> 18) & 1) + ((x >> 19) & 1) + ((x >> 20) & 1) + ((x >> 21) & 1) + ((x >> 22) & 1) + ((x >> 23) & 1) +
    ((x >> 24) & 1) + ((x >> 25) & 1) + ((x >> 26) & 1) + ((x >> 27) & 1) + ((x >> 28) & 1) +
    ((x >> 29) & 1) + ((x >> 30) & 1) + ((x >> 31) & 1);
    // lol

    // hint says count in parallel and log(n) means number of swaps left to do halves each time
    // intuitively i think the shift values would then be >> 31 >> 15 >> 7 >> 3 >> 1 or something
    // recursive????
    // oh count in parallel yeah
    // lets see
    // i have no if statement to handle base case though so probably not

    // ok hit the google divide and conquer approach is very cool but not inuitive to me yet
}
