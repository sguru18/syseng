#include "common.h"
#include "structs.h"
#include <string.h>

/* ===== Linked List Tests ===== */

void test_ll_push_front(void) {
    linked_list_t list;
    ll_init(&list);

    ll_push_front(&list, 10);
    ASSERT_EQ(1, (long long)list.length);
    ASSERT_EQ(10, ll_get(&list, 0));

    ll_push_front(&list, 20);
    ASSERT_EQ(2, (long long)list.length);
    ASSERT_EQ(20, ll_get(&list, 0));
    ASSERT_EQ(10, ll_get(&list, 1));

    ll_push_front(&list, 30);
    ASSERT_EQ(30, ll_get(&list, 0));
    ASSERT_EQ(20, ll_get(&list, 1));
    ASSERT_EQ(10, ll_get(&list, 2));

    ll_free(&list);
}

void test_ll_push_back(void) {
    linked_list_t list;
    ll_init(&list);

    ll_push_back(&list, 10);
    ll_push_back(&list, 20);
    ll_push_back(&list, 30);
    ASSERT_EQ(3, (long long)list.length);
    ASSERT_EQ(10, ll_get(&list, 0));
    ASSERT_EQ(20, ll_get(&list, 1));
    ASSERT_EQ(30, ll_get(&list, 2));

    ll_free(&list);
}

void test_ll_pop_front(void) {
    linked_list_t list;
    ll_init(&list);

    ll_push_front(&list, 10);
    ll_push_front(&list, 20);
    ll_push_front(&list, 30);

    ASSERT_EQ(30, ll_pop_front(&list));
    ASSERT_EQ(2, (long long)list.length);
    ASSERT_EQ(20, ll_pop_front(&list));
    ASSERT_EQ(1, (long long)list.length);
    ASSERT_EQ(10, ll_pop_front(&list));
    ASSERT_EQ(0, (long long)list.length);

    ll_free(&list);
}

void test_ll_contains(void) {
    linked_list_t list;
    ll_init(&list);

    ASSERT_EQ(0, ll_contains(&list, 5));

    ll_push_back(&list, 10);
    ll_push_back(&list, 20);
    ll_push_back(&list, 30);

    ASSERT_EQ(1, ll_contains(&list, 10));
    ASSERT_EQ(1, ll_contains(&list, 20));
    ASSERT_EQ(1, ll_contains(&list, 30));
    ASSERT_EQ(0, ll_contains(&list, 15));
    ASSERT_EQ(0, ll_contains(&list, 0));

    ll_free(&list);
}

void test_ll_remove(void) {
    linked_list_t list;
    ll_init(&list);

    ll_push_back(&list, 10);
    ll_push_back(&list, 20);
    ll_push_back(&list, 30);
    ll_push_back(&list, 40);

    // Remove from middle
    ASSERT_EQ(1, ll_remove(&list, 20));
    ASSERT_EQ(3, (long long)list.length);
    ASSERT_EQ(0, ll_contains(&list, 20));
    ASSERT_EQ(10, ll_get(&list, 0));
    ASSERT_EQ(30, ll_get(&list, 1));
    ASSERT_EQ(40, ll_get(&list, 2));

    // Remove head
    ASSERT_EQ(1, ll_remove(&list, 10));
    ASSERT_EQ(2, (long long)list.length);
    ASSERT_EQ(30, ll_get(&list, 0));

    // Remove tail
    ASSERT_EQ(1, ll_remove(&list, 40));
    ASSERT_EQ(1, (long long)list.length);
    ASSERT_EQ(30, ll_get(&list, 0));

    // Remove nonexistent
    ASSERT_EQ(0, ll_remove(&list, 99));
    ASSERT_EQ(1, (long long)list.length);

    // Remove last element
    ASSERT_EQ(1, ll_remove(&list, 30));
    ASSERT_EQ(0, (long long)list.length);

    ll_free(&list);
}

void test_ll_free(void) {
    linked_list_t list;
    ll_init(&list);

    for (int i = 0; i < 100; i++) {
        ll_push_back(&list, i);
    }
    ASSERT_EQ(100, (long long)list.length);

    ll_free(&list);
    ASSERT_EQ(0, (long long)list.length);
    ASSERT_NULL(list.head);

    // Should be safe to use after free
    ll_push_front(&list, 42);
    ASSERT_EQ(1, (long long)list.length);
    ASSERT_EQ(42, ll_get(&list, 0));
    ll_free(&list);
}

/* ===== Entity Tests ===== */

void test_entity_init(void) {
    entity_t e;
    entity_init(&e, "Goblin", 3, 5, 10);

    ASSERT_STR_EQ("Goblin", e.name);
    ASSERT_EQ(3, e.x);
    ASSERT_EQ(5, e.y);
    ASSERT_EQ(10, e.hp);
    ASSERT_EQ(10, e.max_hp);
    ASSERT_EQ(1, e.alive);
}

void test_entity_take_damage(void) {
    entity_t e;
    entity_init(&e, "Hero", 0, 0, 20);

    entity_take_damage(&e, 5);
    ASSERT_EQ(15, e.hp);
    ASSERT_EQ(1, e.alive);

    entity_take_damage(&e, 10);
    ASSERT_EQ(5, e.hp);
    ASSERT_EQ(1, e.alive);

    // Overkill: hp should not go below 0
    entity_take_damage(&e, 100);
    ASSERT_EQ(0, e.hp);
    ASSERT_EQ(0, e.alive);
}

void test_entity_heal(void) {
    entity_t e;
    entity_init(&e, "Hero", 0, 0, 20);

    entity_take_damage(&e, 15);
    ASSERT_EQ(5, e.hp);

    entity_heal(&e, 3);
    ASSERT_EQ(8, e.hp);

    // Overheal: should clamp to max_hp
    entity_heal(&e, 100);
    ASSERT_EQ(20, e.hp);

    // Dead entities can't heal
    entity_take_damage(&e, 999);
    ASSERT_EQ(0, e.hp);
    ASSERT_EQ(0, e.alive);
    entity_heal(&e, 10);
    ASSERT_EQ(0, e.hp);
}

void test_entity_move(void) {
    entity_t e;
    entity_init(&e, "Scout", 0, 0, 5);

    entity_move(&e, 3, 4);
    ASSERT_EQ(3, e.x);
    ASSERT_EQ(4, e.y);

    entity_move(&e, -1, -2);
    ASSERT_EQ(2, e.x);
    ASSERT_EQ(2, e.y);
}

void test_entity_distance(void) {
    entity_t a, b;
    entity_init(&a, "A", 0, 0, 1);
    entity_init(&b, "B", 3, 4, 1);

    ASSERT_EQ(7, entity_distance(&a, &b));
    ASSERT_EQ(7, entity_distance(&b, &a));

    entity_init(&b, "B", -2, -3, 1);
    ASSERT_EQ(5, entity_distance(&a, &b));

    ASSERT_EQ(0, entity_distance(&a, &a));
}

void test_entity_status(void) {
    entity_t e;
    char buf[128];

    entity_init(&e, "Goblin", 3, 5, 10);
    entity_status(&e, buf);
    ASSERT_STR_EQ("Goblin (3,5) HP: 10/10 [ALIVE]", buf);

    entity_take_damage(&e, 2);
    entity_status(&e, buf);
    ASSERT_STR_EQ("Goblin (3,5) HP: 8/10 [ALIVE]", buf);

    entity_take_damage(&e, 100);
    entity_status(&e, buf);
    ASSERT_STR_EQ("Goblin (3,5) HP: 0/10 [DEAD]", buf);
}

int main(void) {
    printf("=== Structs and Pointers ===\n\n");
    TEST_BEGIN();

    RUN_TEST(test_ll_push_front);
    RUN_TEST(test_ll_push_back);
    RUN_TEST(test_ll_pop_front);
    RUN_TEST(test_ll_contains);
    RUN_TEST(test_ll_remove);
    RUN_TEST(test_ll_free);
    RUN_TEST(test_entity_init);
    RUN_TEST(test_entity_take_damage);
    RUN_TEST(test_entity_heal);
    RUN_TEST(test_entity_move);
    RUN_TEST(test_entity_distance);
    RUN_TEST(test_entity_status);

    TEST_END();
}
