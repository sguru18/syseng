/*
 * Strings and Arrays
 *
 * Implement C string functions from scratch and build a simple dynamic array.
 *
 * KEY C THINGS YOU'LL USE:
 *   - Null terminator: C strings end with '\0'. Your loops need to check for it.
 *   - malloc(size): allocate `size` bytes on the heap. Returns void*.
 *   - free(ptr): release heap memory. Every malloc needs a matching free.
 *   - realloc(ptr, new_size): resize a heap allocation. May move the data.
 *
 * INCLUDES: You need to write your own #include lines now.
 *   - You'll need your own header (strings.h)
 *   - Think about which standard headers provide malloc/free and memcpy.
 *     (Hint: the README for Module 00 lists the common C headers and what's in them)
 */

/* TODO: add your #include lines here */

#include <strings.h>
#include "strings.h"
#include <stdlib.h>

size_t my_strlen(const char *s) {
    // TODO: walk the string until you hit '\0', counting characters.
    //
    // The pattern:
    //   const char *p = s;
    //   while (*p != '\0') { ... p++; }
    //   return p - s;   // pointer arithmetic gives you the count

    const char *p = s;
    while (*p != '\0') {
        p++;
    }
    return p - s;
}

char *my_strcpy(char *dst, const char *src) {
    // TODO: copy characters from src to dst one by one, including the '\0'.
    //
    // Be careful: you must also copy the null terminator!
    char *start = dst;
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    return start;
}

int my_strcmp(const char *a, const char *b) {
    // TODO: compare character by character.
    //
    // - If you reach a difference, return (unsigned char)*a - (unsigned char)*b
    //   (casting to unsigned char is the standard way -- avoids sign issues with chars > 127)
    // - If both strings reach '\0' at the same time, they're equal: return 0
    
    while (*a == *b && *a != '\0') {
        a++;
        b++;
    }
    // done once they are either not equal or they were equal and are both "\0"
    if (*a == *b) { 
        // they are both "\0"
        return 0;
    } 
    // they are not equal, safe to do this because they are both either chars or just "\0" so we can dereference safely
    return (unsigned char)*a - (unsigned char)*b;
}

char *my_strcat(char *dst, const char *src) {
    // TODO: find the end of dst, then copy src there (including '\0').
    //
    // Hint: you already wrote my_strlen and my_strcpy.
    // You can call your own functions here.
    // (void)src;

    int l = my_strlen(dst);
    char *loc = dst + l;
    my_strcpy(loc, src);
    return dst;
}

char *my_strdup(const char *s) {
    // TODO: allocate a new string, copy s into it, return it.
    //
    // 1. Find the length of s
    // 2. malloc(length + 1) bytes (+1 for the null terminator!)
    // 3. Copy s into the new memory
    // 4. Return the new pointer
    //
    // The caller is responsible for calling free() on the result.
    // (void)s;
    int len = my_strlen(s);
    char *dup = malloc((len + 1) * sizeof(char));
    my_strcpy(dup, s);

    return dup;
}

char *str_join(const char *a, const char *b) {
    // TODO: allocate a new string that fits both a and b, concatenate them, return it.
    //
    // Length needed: strlen(a) + strlen(b) + 1
    int len = my_strlen(a) + my_strlen(b) + 1;
    char *result = malloc(len*sizeof(char));
    my_strcpy(result, a);
    my_strcpy(result+my_strlen(a), b); // overwrites the null terminator of a, which is correct
    return result; 
}

void str_reverse(char *s) {
    // TODO: reverse s in place.
    //
    // Find the length, then swap s[0] with s[len-1], s[1] with s[len-2], etc.
    (void)s;
}

int str_is_palindrome(const char *s) {
    // TODO: return 1 if s reads the same forwards and backwards.
    //
    // Hint: you could use str_reverse on a copy, then compare.
    // Or just compare s[i] with s[len-1-i] directly.
    (void)s;
    return 0;
}

/* --- Dynamic integer array (like a very simple std::vector<int>) --- */

void vec_init(int_vec_t *v, size_t initial_capacity) {
    // TODO: allocate the data array and set length/capacity.
    //
    // v->data = malloc(initial_capacity * sizeof(int));
    // v->length = 0;
    // v->capacity = initial_capacity;
    (void)v;
    (void)initial_capacity;
}

void vec_push(int_vec_t *v, int val) {
    // TODO: if length == capacity, double the capacity using realloc().
    // Then store val at data[length] and increment length.
    //
    // realloc(ptr, new_size) resizes the allocation. If it needs to move
    // the data, it copies it for you. Always use the return value:
    //   v->data = realloc(v->data, new_capacity * sizeof(int));
    (void)v;
    (void)val;
}

int vec_pop(int_vec_t *v) {
    // TODO: decrement length, return data[length].
    (void)v;
    return 0;
}

int vec_get(const int_vec_t *v, size_t i) {
    // TODO: return data[i].
    (void)v;
    (void)i;
    return 0;
}

void vec_free(int_vec_t *v) {
    // TODO: free(v->data), then zero out the struct.
    (void)v;
}
