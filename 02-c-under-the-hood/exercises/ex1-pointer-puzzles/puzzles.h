#ifndef PUZZLES_H
#define PUZZLES_H

#include <stdint.h>

struct simple {
    char a;
    int b;
    char c;
};

struct packed_attempt {
    char a;
    char b;
    int c;
};

struct nested {
    char a;
    struct simple inner;
    char b;
};

struct pointer_mix {
    char a;
    char *p;
    int b;
};

struct bitfield_struct {
    unsigned int x : 3;
    unsigned int y : 5;
    unsigned int z : 7;
    unsigned int w : 17;
};

struct tricky {
    char a;
    double d;
    char b;
    int c;
};

/* Prediction function declarations */
int predict_sizeof_simple(void);
int predict_offsetof_simple_b(void);
int predict_offsetof_simple_c(void);

int predict_sizeof_packed_attempt(void);
int predict_offsetof_packed_attempt_c(void);

int predict_sizeof_nested(void);
int predict_offsetof_nested_inner(void);
int predict_offsetof_nested_b(void);

int predict_sizeof_pointer_mix(void);
int predict_offsetof_pointer_mix_p(void);
int predict_offsetof_pointer_mix_b(void);

int predict_sizeof_bitfield_struct(void);

int predict_sizeof_tricky(void);
int predict_offsetof_tricky_d(void);
int predict_offsetof_tricky_b(void);
int predict_offsetof_tricky_c(void);

#endif /* PUZZLES_H */
