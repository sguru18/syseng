/*
 * IEEE 754 Float Bit Surgery
 *
 * Manipulate single-precision floats by operating directly on their
 * 32-bit unsigned integer representation.
 *
 * For special values (NaN, infinity), preserve them or return them unchanged
 * unless the operation specifically transforms them.
 *
 * INCLUDES: Figure out what you need.
 *   You'll need your header. No standard library functions are required --
 *   this is pure bit manipulation.
 */

/* TODO: add your #include lines here */
#include "float_bits.h"

#define SIGN_MASK  0x80000000u
#define EXP_MASK   0x7F800000u
#define FRAC_MASK  0x007FFFFFu
#define EXP_SHIFT  23
#define BIAS       127

/*
 * Example: Negate a float (flip the sign bit).
 * NaN is returned unchanged.
 */
float_bits float_negate(float_bits f) {
    unsigned exp  = (f >> EXP_SHIFT) & 0xFF;
    unsigned frac = f & FRAC_MASK;
    if (exp == 0xFF && frac != 0) return f;
    return f ^ SIGN_MASK;
}

/* TODO: Return the absolute value of f (clear the sign bit). NaN unchanged. */
float_bits float_abs(float_bits f) {
    // f is something like 1 10000000 10010010000111111011011
    // the sign bit is the very first one
    // i can and it with 0 to make sure the first one becomes 0
    // but i have to preserve the rest
    // what if i and with 0 11111111 11111111111...
    // whatever was 0 will stay 0, whatever was 1 will stay 1
    // use hex
    return f & 0x7FFFFFFF;
}

/*
 * TODO: Return f * 2.0.
 * Think about what "multiply by 2" means for normalized, denormalized,
 * zero, infinity, and NaN.
 */
float_bits float_twice(float_bits f) {
    // return 0;
    // need to increment the exponent by 1 
    // this is done by left shifting from the mantissa for denorm
    // the mantissa is at the end anyway so use f << 1
    // preserve sign bit though
    int exponent = (f & 0x7F800000) >> 23;
    if (exponent == 0) { // denormalized and 0
        int is_negative = f & 0x80000000; // 0x80000000 is sign bit is 1
        f = f << 1; // multiply the rest by 2
        f = f & 0x7FFFFFFF; // get rid of sign bit
        f = f | is_negative;
        return f;
    } else if (exponent == 0xFF) {
        return f;
    } else {
        exponent += 1;
        exponent = exponent << 23;
        f = f & ~0x7F800000; // clear the bits of the exponent 
        f = f | exponent; // put the new exponent in
        return f;
    }
    // this was actually very cool 
}

/*
 * TODO: Return f / 2.0.
 * The reverse of float_twice, but watch the boundary where a normalized
 * number becomes denormalized (exponent goes from 1 to 0).
 */
float_bits float_half(float_bits f) {
    // so we want to decrement the exponent by 1
    // if exponent was already 0 then we want to rightshift the mantissa
    // then the sign bit might move into the second bit which should be 0
    // so store the sign bit as above and replace it 
    // and make sure second bit is 0
    // i think the normalized -> denormalized boundary is handled by rightshift
    // because 1 goes into the mantissa and exponent is 0
    int exponent = (f & 0x7F800000) >> 23;
    if (exponent == 0) {
        int is_negative = f & 0x80000000; // 0x80000000 if negative else 0
        f = f >> 1;
        f = f & 0x7FFFFFFF; // get rid of sign bit
        f = f & 0xBFFFFFFF; // make sure the second bit is 0
        f = f | is_negative; // restore correct sign bit
        return f;
    } else if (exponent == 0xFF) {
        return f; // infinity and NaN stay the same I think
    } else {
        // rightshift exponent 
        exponent = exponent - 1;
        exponent = exponent << 23;
        f = f & (~0x7F800000); // clear the sign bits in f
        f = f | exponent; // put the updated exponent in
        return f;
    }
}

/* TODO: Return 1 if f represents NaN, 0 otherwise. */
int float_is_nan(float_bits f) {
    return 0;
}

/*
 * TODO: Convert a signed 32-bit integer to its IEEE 754 float representation.
 *
 * This is the hardest function. Think about:
 *   - Special case: x == 0
 *   - Sign handling (watch out for INT_MIN: its abs doesn't fit in a signed int)
 *   - Finding the most significant 1-bit to determine the exponent
 *   - Extracting the 23-bit mantissa (minus the implicit leading 1)
 *   - Rounding: when you discard low bits, round to nearest even
 */
float_bits int_to_float(int x) {
    return 0;
}
