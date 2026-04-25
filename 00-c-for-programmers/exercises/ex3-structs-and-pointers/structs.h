#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>

/* ===== Part 1: Linked List ===== */

typedef struct ll_node {
    int value;
    struct ll_node *next;  /* self-referential: a node points to another node */
} ll_node_t;

typedef struct {
    ll_node_t *head;
    size_t length;
} linked_list_t;

/* Initialize an empty list */
void ll_init(linked_list_t *list);

/* Push a value to the front of the list (O(1)) */
void ll_push_front(linked_list_t *list, int value);

/* Push a value to the back of the list (O(n) -- we don't keep a tail pointer) */
void ll_push_back(linked_list_t *list, int value);

/* Pop the front value. Returns the value. Behavior undefined if list is empty. */
int ll_pop_front(linked_list_t *list);

/* Return 1 if value is in the list, 0 otherwise */
int ll_contains(const linked_list_t *list, int value);

/* Return the value at index i (0-based). Behavior undefined if i >= length. */
int ll_get(const linked_list_t *list, size_t i);

/* Remove the first occurrence of value. Return 1 if removed, 0 if not found. */
int ll_remove(linked_list_t *list, int value);

/* Free all nodes in the list and reset it to empty */
void ll_free(linked_list_t *list);

/* ===== Part 2: Entity System ===== */

typedef struct {
    char name[64];       /* fixed-size buffer for the name (no heap allocation needed) */
    int x, y;            /* position */
    int hp;              /* hit points */
    int max_hp;
    int alive;           /* 1 if alive, 0 if dead */
} entity_t;

/* Initialize an entity with the given name, position, and max HP */
void entity_init(entity_t *e, const char *name, int x, int y, int max_hp);

/* Apply damage to the entity. HP should not go below 0. Set alive=0 if HP reaches 0. */
void entity_take_damage(entity_t *e, int amount);

/* Heal the entity. HP should not exceed max_hp. Only works if alive. */
void entity_heal(entity_t *e, int amount);

/* Move the entity by (dx, dy) */
void entity_move(entity_t *e, int dx, int dy);

/* Return the Manhattan distance between two entities: |a.x - b.x| + |a.y - b.y| */
int entity_distance(const entity_t *a, const entity_t *b);

/* Write a status string into buf (buf is at least 128 bytes).
 * Format: "Name (x,y) HP: hp/max_hp [ALIVE]" or "Name (x,y) HP: 0/max_hp [DEAD]"
 * Example: "Goblin (3,5) HP: 8/10 [ALIVE]" */
void entity_status(const entity_t *e, char *buf);

#endif /* STRUCTS_H */
