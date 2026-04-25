#ifndef SORT_H
#define SORT_H

#include <stddef.h>

typedef int (*comparator_fn)(const void *a, const void *b);

/*
 * Generic sort: sorts `arr` of `n` elements, each `elem_size` bytes,
 * using `cmp` to compare elements. You choose the sorting algorithm.
 */
void generic_sort(void *arr, size_t n, size_t elem_size, comparator_fn cmp);

/* Generic swap helper (provided for you) */
void generic_swap(void *a, void *b, size_t size);

/* Comparators -- YOU implement these */
int cmp_int_asc(const void *a, const void *b);
int cmp_int_desc(const void *a, const void *b);
int cmp_string_asc(const void *a, const void *b);
int cmp_string_by_length(const void *a, const void *b);

/* Bonus: a dual comparator that uses a primary, then falls back to secondary on ties */
typedef struct {
    comparator_fn primary;
    comparator_fn secondary;
} dual_comparator_t;

int cmp_dual(const void *ctx, const void *a, const void *b);

#endif /* SORT_H */
