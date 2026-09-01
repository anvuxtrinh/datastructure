#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;

static void setup() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
}

static void teardown() {
    vec_free(&vec);
}

static void test_vec_push_null() {
    int item = 42;
    int ret = vec_push(NULL, &item);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

    ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

static void test_vec_push_null_data() {
    int ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

static void test_vec_push_valid() {
    int item = 42;
    int ret = vec_push(&vec, &item);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *retrieved_item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
    ASSERT_EQ(item, *retrieved_item, "Expected condition to hold");
}

static void test_vec_push_struct() {
    struct Point {
        int x;
        int y;
    };

    struct Point p = { .x = 10, .y = 20 };

    vec_t vec_struct;
    int ret = vec_init(&vec_struct, sizeof(struct Point), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_push(&vec_struct, &p);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec_struct.size, "Expected condition to hold");

    struct Point *retrieved_point = (struct Point *)vec_at(&vec_struct, 0);
    ASSERT_NOT_NULL(retrieved_point, "Expected pointer to be non-NULL");
    ASSERT_EQ(p.x, retrieved_point->x, "Expected condition to hold");
    ASSERT_EQ(p.y, retrieved_point->y, "Expected condition to hold");

    vec_free(&vec_struct);
}

static void test_vec_push_multiple() {
    int items[] = {1, 2, 3, 4, 5};
    size_t num_items = sizeof(items) / sizeof(items[0]);

    for(size_t i = 0; i < num_items; i++) {
        int ret = vec_push(&vec, &items[i]);
        ASSERT_EQ(0, ret, "Expected condition to hold");
        ASSERT_EQ(i + 1, vec.size, "Expected condition to hold");
    }

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
        ASSERT_EQ(items[i], *retrieved_item, "Expected condition to hold");
    }
}

static void test_vec_push_resize() {
    size_t initial_capacity = vec.cap;
    size_t num_items = 6;

    for(size_t i = 0; i < num_items; i++) {
        int item = (int)i;
        int ret = vec_push(&vec, &item);
        ASSERT_EQ(0, ret, "Expected condition to hold");
        ASSERT_EQ(i + 1, vec.size, "Expected condition to hold");
    }

    ASSERT_TRUE(vec.cap > initial_capacity, "Expected condition to be true");

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
        ASSERT_EQ((int)i, *retrieved_item, "Expected condition to hold");
    }
}

void run_vec_push_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_push_null);
    RUN_TEST(test_vec_push_null_data);
    RUN_TEST(test_vec_push_valid);
    RUN_TEST(test_vec_push_struct);
    RUN_TEST(test_vec_push_multiple);
    RUN_TEST(test_vec_push_resize);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}