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

void test_remove_null() {
    int ret = vec_remove(NULL, 0);
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

void test_remove_out_of_bounds() {
    int ret = vec_remove(&vec, 0);
    ASSERT_EQ(VEC_ERR_OUT_OF_BOUNDS, ret);
}

void test_remove_first() {
    Vec vec;
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 0);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(1, vec.size);

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item);
    ASSERT_EQ(value2, *item);

    vec_free(&vec);
}

void test_remove_last() {
    Vec vec;
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(1, vec.size);

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item);
    ASSERT_EQ(value1, *item);

    vec_free(&vec);
}

void test_remove_middle() {
    Vec vec;
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);

    int value1 = 42;
    int value2 = 84;
    int value3 = 126;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);
    vec_push(&vec, &value3);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(2, vec.size);

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1);
    ASSERT_EQ(value1, *item1);

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2);
    ASSERT_EQ(value3, *item2);

    vec_free(&vec);
}

void run_vec_remove_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_remove_null);
    RUN_TEST(test_remove_out_of_bounds);
    RUN_TEST(test_remove_first);
    RUN_TEST(test_remove_last);
    RUN_TEST(test_remove_middle);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}