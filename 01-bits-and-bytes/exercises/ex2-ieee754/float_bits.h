#ifndef FLOAT_BITS_H
#define FLOAT_BITS_H

typedef unsigned int float_bits;

float_bits float_negate(float_bits f);
float_bits float_abs(float_bits f);
float_bits float_twice(float_bits f);
float_bits float_half(float_bits f);
int float_is_nan(float_bits f);
float_bits int_to_float(int x);

#endif /* FLOAT_BITS_H */
