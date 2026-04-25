/*
 * Generic Sort with Function Pointers
 *
 * Implement a type-agnostic sort and comparator functions.
 * Read sort.h for the full interface.
 *
 * INCLUDES: Figure out what you need. You'll definitely need your header.
 *   Think about what standard functions you'll use (string comparison, strlen, memcpy).
 */

/* TODO: add your #include lines here */

/*
 * Provided: swaps `size` bytes between the memory at `a` and `b`.
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
 * TODO: Sort arr (n elements, each elem_size bytes) using the comparator cmp.
 *
 * To access element i:  (char *)arr + i * elem_size
 * To compare elements:  cmp(ptr_to_i, ptr_to_j)
 * To swap elements:     generic_swap(ptr_to_i, ptr_to_j, elem_size)
 *
 * Any correct sorting algorithm works.
 */
void generic_sort(void *arr, size_t n, size_t elem_size, comparator_fn cmp) {
    (void)arr; (void)n; (void)elem_size; (void)cmp;
}

/* TODO: Compare two ints ascending. a and b point to ints in the array. */
int cmp_int_asc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/* TODO: Compare two ints descending. */
int cmp_int_desc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/*
 * TODO: Compare two strings alphabetically.
 * The array is char*[], so a and b are pointers to char* (i.e., char**).
 * Dereference once to get the actual string, then compare.
 */
int cmp_string_asc(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/* TODO: Compare two strings by length (shorter first). Break ties alphabetically. */
int cmp_string_by_length(const void *a, const void *b) {
    (void)a; (void)b;
    return 0;
}

/* Provided: dual comparator -- primary, then secondary on tie. */
int cmp_dual(const void *ctx, const void *a, const void *b) {
    const dual_comparator_t *dc = (const dual_comparator_t *)ctx;
    int result = dc->primary(a, b);
    if (result != 0) return result;
    return dc->secondary(a, b);
}
