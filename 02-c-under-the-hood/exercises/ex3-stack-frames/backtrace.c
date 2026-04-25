#include "backtrace.h"
#include <stdio.h>

/*
 * Print a captured backtrace to stdout.
 * Shows frame index, return address, and frame pointer for each frame.
 */
void print_backtrace(const stack_frame_t *frames, int count) {
    printf("Backtrace (%d frames):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  #%-2d  return_addr=%p  frame_ptr=%p\n",
               i, frames[i].return_address, frames[i].frame_pointer);
    }
}

/*
 * TODO: Walk the call stack by following frame pointers.
 *
 * On ARM64 (Apple Silicon), the frame pointer chain works like this:
 *
 *   - Get the current frame pointer: __builtin_frame_address(0)
 *     This returns a void* to the current function's saved frame pointer.
 *
 *   - At that address, you'll find two 64-bit values laid out as:
 *       fp[0] = previous frame's frame pointer  (the next link in the chain)
 *       fp[1] = return address                   (where this frame returns to)
 *
 *   - To walk the chain, treat the frame pointer as a (void **):
 *       void **fp = (void **)__builtin_frame_address(0);
 *       previous_fp     = fp[0]
 *       return_address  = fp[1]
 *
 *   - Then follow the chain:
 *       fp = (void **)fp[0]   // move to the previous frame
 *
 *   - Stop when:
 *       (a) fp is NULL, or
 *       (b) you've captured max_frames frames, or
 *       (c) fp looks invalid (optional safety check)
 *
 * Fill in `frames[i].frame_pointer` and `frames[i].return_address` for
 * each frame, and return the number of frames captured.
 */
int capture_backtrace(stack_frame_t *frames, int max_frames) {
    (void)frames; (void)max_frames;
    return 0;
}
