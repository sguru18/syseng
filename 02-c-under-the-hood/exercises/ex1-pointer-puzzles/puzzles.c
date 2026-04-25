/*
 * Pointer Puzzles: Struct Layout Predictions
 *
 * Read each struct definition in puzzles.h, mentally work out the
 * sizeof and offsetof values, then replace -1 with your prediction.
 *
 * INCLUDES: You need to include your header and whatever gives you offsetof().
 */

/* TODO: add your #include lines here */

/* struct simple { char a; int b; char c; } */
int predict_sizeof_simple(void)          { return -1; }
int predict_offsetof_simple_b(void)      { return -1; }
int predict_offsetof_simple_c(void)      { return -1; }

/* struct packed_attempt { char a; char b; int c; } */
int predict_sizeof_packed_attempt(void)  { return -1; }
int predict_offsetof_packed_attempt_c(void) { return -1; }

/* struct nested { char a; struct simple inner; char b; } */
int predict_sizeof_nested(void)          { return -1; }
int predict_offsetof_nested_inner(void)  { return -1; }
int predict_offsetof_nested_b(void)      { return -1; }

/* struct pointer_mix { char a; char *p; int b; } */
int predict_sizeof_pointer_mix(void)     { return -1; }
int predict_offsetof_pointer_mix_p(void) { return -1; }
int predict_offsetof_pointer_mix_b(void) { return -1; }

/* struct bitfield_struct { x:3, y:5, z:7, w:17 } */
int predict_sizeof_bitfield_struct(void) { return -1; }

/* struct tricky { char a; double d; char b; int c; } */
int predict_sizeof_tricky(void)          { return -1; }
int predict_offsetof_tricky_d(void)      { return -1; }
int predict_offsetof_tricky_b(void)      { return -1; }
int predict_offsetof_tricky_c(void)      { return -1; }
