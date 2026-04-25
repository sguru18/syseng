#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

/* Return the length of s (not counting the null terminator).
 * Implement this yourself -- don't call strlen(). */
size_t my_strlen(const char *s);

/* Copy src into dst, including the null terminator.
 * Return dst.
 * Implement yourself -- don't call strcpy(). */
char *my_strcpy(char *dst, const char *src);

/* Return 0 if a and b are equal, negative if a < b, positive if a > b.
 * Compare character by character (lexicographic order).
 * Implement yourself -- don't call strcmp(). */
int my_strcmp(const char *a, const char *b);

/* Concatenate src onto the end of dst. dst must have enough space.
 * Return dst.
 * Implement yourself -- don't call strcat(). */
char *my_strcat(char *dst, const char *src);

/* Return a newly heap-allocated copy of s (caller must free it).
 * This is like C's strdup() which isn't always available. */
char *my_strdup(const char *s);

/* Return a new heap-allocated string that is the concatenation of a and b.
 * Caller must free the result. */
char *str_join(const char *a, const char *b);

/* Reverse s in place. */
void str_reverse(char *s);

/* Return 1 if s is a palindrome, 0 otherwise.
 * "racecar" -> 1, "hello" -> 0, "" -> 1 */
int str_is_palindrome(const char *s);

/* Dynamic integer array */
typedef struct {
    int *data;       /* heap-allocated array of ints */
    size_t length;   /* number of elements currently stored */
    size_t capacity; /* number of elements the array can hold before resizing */
} int_vec_t;

/* Initialize a vector with the given initial capacity. */
void vec_init(int_vec_t *v, size_t initial_capacity);

/* Append val to the end of the vector.
 * If the vector is full, double its capacity. */
void vec_push(int_vec_t *v, int val);

/* Remove and return the last element. Behavior undefined if empty. */
int vec_pop(int_vec_t *v);

/* Return the element at index i. */
int vec_get(const int_vec_t *v, size_t i);

/* Free the vector's heap memory. */
void vec_free(int_vec_t *v);

#endif /* STRINGS_H */
