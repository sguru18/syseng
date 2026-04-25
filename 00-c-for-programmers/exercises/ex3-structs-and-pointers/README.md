# Exercise 3: Structs and Pointers

## Structs: Classes Without Methods

In C, a `struct` is just a bundle of named fields. No methods, no constructors, no destructors, no inheritance. Just data.

```c
struct point {
    double x;
    double y;
};

struct point p;       // declare (no 'new', it's on the stack)
p.x = 3.0;           // use the dot operator
p.y = 4.0;
```

You can use `typedef` to avoid writing `struct` everywhere:

```c
typedef struct {
    double x;
    double y;
} point_t;            // now you can just write: point_t p;
```

## The `->` Operator

When you have a pointer to a struct, use `->` instead of `.`:

```c
void move(point_t *p, double dx, double dy) {
    p->x += dx;       // (*p).x += dx; -- same thing, just cleaner
    p->y += dy;
}

point_t origin = {0.0, 0.0};
move(&origin, 1.0, 2.0);    // pass the ADDRESS of origin
// origin is now {1.0, 2.0}
```

Why pointers? Two reasons:
1. **Mutation**: C passes everything by value. Without a pointer, the function gets a copy and the original doesn't change.
2. **Efficiency**: passing a large struct by value copies every byte. A pointer is always 8 bytes.

## The Pattern: Struct + Functions

C doesn't have methods, but the convention is clear:

```c
// C++ style:  player.take_damage(10);
// C style:    player_take_damage(&player, 10);
```

Every "method" is just a function whose first argument is a pointer to the struct. This is actually what C++ does under the hood -- the `this` pointer.

## Initialization Patterns

```c
// Stack allocation (most common for small/temporary structs)
point_t p = {.x = 1.0, .y = 2.0};   // designated initializers (C99+)

// Heap allocation (when the struct outlives the current scope)
point_t *p = malloc(sizeof(point_t));
p->x = 1.0;
p->y = 2.0;
// ... use p ...
free(p);

// Common pattern: init function
void point_init(point_t *p, double x, double y) {
    p->x = x;
    p->y = y;
}
```

## The Exercise

Build a linked list and a simple "entity" system -- both common C patterns you'll see everywhere in systems code. This exercise bridges you from "I know the syntax" to "I can write idiomatic C."

## Building & Testing

```bash
make test
```

## Think Further

After finishing, notice: the linked list and the entity system both follow the same pattern. A struct for data, standalone functions that take a pointer to it, an init/destroy lifecycle. This is the C equivalent of a class. Every C library you'll ever read (SQLite, Linux kernel, Redis, nginx) uses exactly this pattern.
