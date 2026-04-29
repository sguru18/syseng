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

#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include "../ex1-hello-c/hello.h"
#include <string.h>

/* ===== Part 1: Linked List ===== */

void ll_init(linked_list_t *list) {
    list->head = NULL;
    list->length = 0;
}

void ll_push_front(linked_list_t *list, int value) {
    // TODO: allocate a new node, set its value, point it at the current head,
    //       then update head to the new node. Increment length.
    
    ll_node_t *n = malloc(sizeof(ll_node_t));
    if (n == NULL) { } // handle error
    n -> value = value;
    n -> next = list -> head;
    list -> head = n;
    (list -> length)++;
}

void ll_push_back(linked_list_t *list, int value) {
    // TODO: allocate a new node with next = NULL.
    //       If list is empty, set head to the new node.
    //       Otherwise, walk to the last node and set its next to the new node.
    //       Increment length.
    
    ll_node_t *n = malloc(sizeof(ll_node_t));
    if (n == NULL) {} // handle error
    n -> next = NULL;
    n -> value = value;
    if (list -> length == 0) {
        list -> head = n;
    } else {
        // walk to the last node and set its next to the new node
        ll_node_t *cur = list -> head;
        while (cur -> next) {
            cur = cur -> next;
        }
        cur -> next = n;
    }
    list -> length++;
}

int ll_pop_front(linked_list_t *list) {
    // TODO: save head's value, advance head to head->next,
    //       free the old head node, decrement length, return the saved value.
    //
    // Important: free the node AFTER reading its value, not before!
    
    int val = list->head->value;
    ll_node_t *cur = list -> head;
    list -> head = cur -> next;
    free(cur);
    list -> length--;
    return val;
}

int ll_contains(const linked_list_t *list, int value) {
    // TODO: walk the list, return 1 if you find a node with this value.
    
    ll_node_t *cur = list -> head;
    while (cur) {
        if (cur -> value == value) return 1;
        cur = cur -> next;
    }
    return 0;
}

int ll_get(const linked_list_t *list, size_t i) {
    // TODO: walk i steps from head, return that node's value.
    
    ll_node_t *cur = list -> head;
    for (size_t steps = 0; steps < i; ++steps) {
        cur = cur -> next;
    }
    if (!cur) {} // handle error
    return cur -> value;
}

int ll_remove(linked_list_t *list, int value) {
    // TODO: find the first node with this value and remove it.
    //
    // Tricky part: you need a pointer to the PREVIOUS node so you can
    // update prev->next to skip over the removed node.
    // Special case: removing the head node (there's no previous).
    // Don't forget to free() the removed node and decrement length.

    // make sure safe to check head
    if (list -> length == 0) {return 0;}

    // handle special case outside of main logic
    if (list -> head -> value == value) {
        ll_node_t* old = list -> head;
        list -> head = old -> next;
        free(old);
        list -> length--;
        return 1; // we don't have to store it from the node because they match obviously
    }

    ll_node_t *prev = list -> head;
    ll_node_t *cur = list -> head -> next;
    while (cur) {
        if (cur -> value == value) {
            prev -> next = cur -> next;
            free(cur);
            list -> length--;
            return 1;
        }
        prev = cur;
        cur = cur -> next;
    }

    return 0;
}

void ll_free(linked_list_t *list) {
    // TODO: walk the list and free every node.
    //
    // Careful: save cur->next BEFORE freeing cur!
    // Then reset head to NULL and length to 0.
    
    ll_node_t *cur = list -> head;
    while (cur) {
        ll_node_t *prev = cur;
        cur = cur -> next;
        free(prev);
    }

    list -> head = NULL;
    list -> length = 0;
}

/* ===== Part 2: Entity System ===== */

void entity_init(entity_t *e, const char *name, int x, int y, int max_hp) {
    // TODO: copy name into e->name (use strncpy to avoid overflow).
    //       Set position, hp = max_hp, max_hp, alive = 1.
    //
    // Gotcha: strncpy does NOT guarantee null termination if src is too long.
    // Always set the last byte manually: e->name[sizeof(e->name) - 1] = '\0';
    
    strncpy(e->name, name, sizeof(e->name)-1);
    e->name[sizeof(e->name) - 1] = '\0';

    e->hp = max_hp;
    e->max_hp = max_hp;
    e->alive = 1;
    e->x = x;
    e->y = y;
}

void entity_take_damage(entity_t *e, int amount) {
    // TODO: subtract amount from hp. Clamp hp to 0 minimum.
    //       If hp reaches 0, set alive = 0.
    
    e -> hp -= amount;
    if (e -> hp <= 0) {
        e -> hp = 0;
        e -> alive = 0;
    }
}

void entity_heal(entity_t *e, int amount) {
    // TODO: if not alive, do nothing. Otherwise add amount to hp.
    //       Clamp hp to max_hp.
    
    if (e->alive) {
        e -> hp += amount;
        if (e -> hp > e->max_hp) { e->hp = e->max_hp; }
    }
}

void entity_move(entity_t *e, int dx, int dy) {
    // TODO: add dx to x, dy to y.
    
    e->x += dx;
    e->y += dy;
}

int entity_distance(const entity_t *a, const entity_t *b) {
    // TODO: return Manhattan distance: |a->x - b->x| + |a->y - b->y|
    
    return (abs(a->x - b->x) + abs(a->y - b->y));
}

void entity_status(const entity_t *e, char *buf) {
    // TODO: write status string into buf using sprintf.
    //       Format: "Name (x,y) HP: hp/max_hp [ALIVE]" or "[DEAD]"
    
    if (e->alive) { sprintf(buf, "%s (%d,%d) HP: %d/%d [ALIVE]", e->name, e->x, e->y, e->hp, e->max_hp); }
    else { sprintf(buf, "%s (%d,%d) HP: %d/%d [DEAD]", e->name, e->x, e->y, e->hp, e->max_hp); }
}
