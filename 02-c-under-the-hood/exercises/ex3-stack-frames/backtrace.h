#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *return_address;
    void *frame_pointer;
} stack_frame_t;

/*
 * Walk the call stack starting from the current frame.
 * Fill `frames` with up to `max_frames` stack frames.
 * Return the number of frames actually captured.
 */
int capture_backtrace(stack_frame_t *frames, int max_frames);

/* Print a captured backtrace (provided for you) */
void print_backtrace(const stack_frame_t *frames, int count);

#endif /* BACKTRACE_H */
