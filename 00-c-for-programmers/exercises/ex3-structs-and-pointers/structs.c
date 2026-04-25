/*
 * Structs and Pointers
 *
 * Part 1: A singly-linked list (the fundamental dynamic data structure in C)
 * Part 2: An "entity" system (the struct + functions pattern used everywhere)
 *
 * INCLUDES: Write your own. You'll need:
 *   - Your header file (structs.h)
 *   - Headers for: malloc/free, printf/sprintf, strncpy/strlen
 */

/* TODO: add your #include lines here */

/* ===== Part 1: Linked List ===== */

void ll_init(linked_list_t *list) {
    list->head = NULL;
    list->length = 0;
}

void ll_push_front(linked_list_t *list, int value) {
    // TODO: allocate a new node, set its value, point it at the current head,
    //       then update head to the new node. Increment length.
    (void)list;
    (void)value;
}

void ll_push_back(linked_list_t *list, int value) {
    // TODO: allocate a new node with next = NULL.
    //       If list is empty, set head to the new node.
    //       Otherwise, walk to the last node and set its next to the new node.
    //       Increment length.
    (void)list;
    (void)value;
}

int ll_pop_front(linked_list_t *list) {
    // TODO: save head's value, advance head to head->next,
    //       free the old head node, decrement length, return the saved value.
    //
    // Important: free the node AFTER reading its value, not before!
    (void)list;
    return 0;
}

int ll_contains(const linked_list_t *list, int value) {
    // TODO: walk the list, return 1 if you find a node with this value.
    (void)list;
    (void)value;
    return 0;
}

int ll_get(const linked_list_t *list, size_t i) {
    // TODO: walk i steps from head, return that node's value.
    (void)list;
    (void)i;
    return 0;
}

int ll_remove(linked_list_t *list, int value) {
    // TODO: find the first node with this value and remove it.
    //
    // Tricky part: you need a pointer to the PREVIOUS node so you can
    // update prev->next to skip over the removed node.
    // Special case: removing the head node (there's no previous).
    // Don't forget to free() the removed node and decrement length.
    (void)list;
    (void)value;
    return 0;
}

void ll_free(linked_list_t *list) {
    // TODO: walk the list and free every node.
    //
    // Careful: save cur->next BEFORE freeing cur!
    // Then reset head to NULL and length to 0.
    (void)list;
}

/* ===== Part 2: Entity System ===== */

void entity_init(entity_t *e, const char *name, int x, int y, int max_hp) {
    // TODO: copy name into e->name (use strncpy to avoid overflow).
    //       Set position, hp = max_hp, max_hp, alive = 1.
    //
    // Gotcha: strncpy does NOT guarantee null termination if src is too long.
    // Always set the last byte manually: e->name[sizeof(e->name) - 1] = '\0';
    (void)e;
    (void)name;
    (void)x;
    (void)y;
    (void)max_hp;
}

void entity_take_damage(entity_t *e, int amount) {
    // TODO: subtract amount from hp. Clamp hp to 0 minimum.
    //       If hp reaches 0, set alive = 0.
    (void)e;
    (void)amount;
}

void entity_heal(entity_t *e, int amount) {
    // TODO: if not alive, do nothing. Otherwise add amount to hp.
    //       Clamp hp to max_hp.
    (void)e;
    (void)amount;
}

void entity_move(entity_t *e, int dx, int dy) {
    // TODO: add dx to x, dy to y.
    (void)e;
    (void)dx;
    (void)dy;
}

int entity_distance(const entity_t *a, const entity_t *b) {
    // TODO: return Manhattan distance: |a->x - b->x| + |a->y - b->y|
    (void)a;
    (void)b;
    return 0;
}

void entity_status(const entity_t *e, char *buf) {
    // TODO: write status string into buf using sprintf.
    //       Format: "Name (x,y) HP: hp/max_hp [ALIVE]" or "[DEAD]"
    (void)e;
    (void)buf;
}
