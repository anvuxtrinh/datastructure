#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static Vec vec;

static void setup() {
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
}

static void teardown() {
    vec_free(&vec);
}

static void test_vec_push_null() {
    int item = 42;
    int ret = vec_push(NULL, &item);
    ASSERT_EQ(VEC_ERR_NULL, ret);

    ret = vec_push(&vec, NULL);
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

static void test_vec_push_null_data() {
    int ret = vec_push(&vec, NULL);
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

static void test_vec_push_valid() {
    int item = 42;
    int ret = vec_push(&vec, &item);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(1, vec.size);

    int *retrieved_item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(retrieved_item);
    ASSERT_EQ(item, *retrieved_item);
}

static void test_vec_push_struct() {
    struct Point {
        int x;
        int y;
    };

    struct Point p = { .x = 10, .y = 20 };

    Vec vec_struct;
    int ret = vec_init(&vec_struct, 0, sizeof(struct Point));
    ASSERT_EQ(VEC_SUCCESS, ret);

    ret = vec_push(&vec_struct, &p);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(1, vec_struct.size);

    struct Point *retrieved_point = (struct Point *)vec_at(&vec_struct, 0);
    ASSERT_NOT_NULL(retrieved_point);
    ASSERT_EQ(p.x, retrieved_point->x);
    ASSERT_EQ(p.y, retrieved_point->y);

    vec_free(&vec_struct);
}

static void test_vec_push_multiple() {
    int items[] = {1, 2, 3, 4, 5};
    size_t num_items = sizeof(items) / sizeof(items[0]);

    for(size_t i = 0; i < num_items; i++) {
        int ret = vec_push(&vec, &items[i]);
        ASSERT_EQ(VEC_SUCCESS, ret);
        ASSERT_EQ(i + 1, vec.size);
    }

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item);
        ASSERT_EQ(items[i], *retrieved_item);
    }
}

static void test_vec_push_resize() {
    size_t initial_capacity = vec.cap;
    size_t num_items = initial_capacity + 2; // Push more items than initial capacity

    for(size_t i = 0; i < num_items; i++) {
        int item = (int)i;
        int ret = vec_push(&vec, &item);
        ASSERT_EQ(VEC_SUCCESS, ret);
        ASSERT_EQ(i + 1, vec.size);
    }

    ASSERT_TRUE(vec.cap > initial_capacity); // Ensure capacity has increased

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item);
        ASSERT_EQ((int)i, *retrieved_item);
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