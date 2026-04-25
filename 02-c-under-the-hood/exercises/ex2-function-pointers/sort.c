#include "sort.h"
#include <string.h>

/*
 * Swaps `size` bytes between the memory at `a` and `b`.
 * Uses a small stack buffer to avoid malloc for typical element sizes.
 */
void generic_swap(void *a, void *b, size_t size) {
    char tmp[256];
    if (size <= sizeof(tmp)) {
        memcpy(tmp, a, size);
        memcpy(a, b, size);
        memcpy(b, tmp, size);
    } else {
        char *pa = (char *)a;
        char *pb = (char *)b;
        for (size_t i = 0; i < size; i++) {
            char t = pa[i];
            pa[i] = pb[i];
            pb[i] = t;
        }
    }
}

/*
 * TODO: Implement a generic sorting algorithm.
 *
 * Sort `arr` (an array of `n` elements, each `elem_size` bytes) using
 * the comparator `cmp` to determine ordering.
 *
 * To access element i:   (char *)arr + i * elem_size
 * To compare elements:   cmp(elem_i, elem_j)
 *   -> returns negative if i < j, zero if equal, positive if i > j
 * To swap elements:      generic_swap(elem_i, elem_j, elem_size)
 *
 * Any correct O(n^2) or O(n log n) algorithm works. Selection sort or
 * bubble sort is fine -- the point is making it generic with void*.
 */
void generic_sort(void *arr, size_t n, size_t elem_size, comparator_fn cmp) {
    (void)arr; (void)n; (void)elem_size; (void)cmp;
}

/*
 * TODO: Compare two ints in ascending order.
 *
 * `a` and `b` each point to an int in the array.
 * Return negative if *a < *b, zero if equal, positive if *a > *b.
 */
int cmp_int_asc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/*
 * TODO: Compare two ints in descending order.
 *
 * Same as ascending, but reversed.
 */
int cmp_int_desc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/*
 * TODO: Compare two strings alphabetically (ascending).
 *
 * IMPORTANT: `a` and `b` point to elements in the array.
 * The array is char*[], so each element is a char*.
 * That means a and b are char** -- pointers to string pointers.
 * Cast to (const char **) and dereference to get the actual strings.
 */
int cmp_string_asc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/*
 * TODO: Compare two strings by length (shorter first).
 *
 * Same pointer logic as cmp_string_asc.
 * If lengths are equal, break ties alphabetically.
 */
int cmp_string_by_length(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/*
 * Bonus: Dual comparator.
 * Uses `ctx` (a dual_comparator_t*) to first compare by primary.
 * If primary returns 0 (tie), falls back to secondary.
 */
int cmp_dual(const void *ctx, const void *a, const void *b) {
    const dual_comparator_t *dc = (const dual_comparator_t *)ctx;
    int result = dc->primary(a, b);
    if (result != 0) return result;
    return dc->secondary(a, b);
}
