/*
 * Stack Frame Walker
 *
 * Implement capture_backtrace: walk the call stack by following frame pointers.
 * Read backtrace.h for the interface and the README for how frames work on ARM64.
 *
 * INCLUDES: Figure out what you need.
 */

/* TODO: add your #include lines here */

/* Provided: print a captured backtrace to stdout. */
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
 * Start with __builtin_frame_address(0) to get the current frame pointer.
 * On ARM64, treat the frame pointer as a void**:
 *   fp[0] = previous frame pointer, fp[1] = return address
 * Follow the chain until fp is NULL or you've captured max_frames.
 */
int capture_backtrace(stack_frame_t *frames, int max_frames) {
    (void)frames; (void)max_frames;
    return 0;
}
