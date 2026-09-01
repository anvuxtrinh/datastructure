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

void test_remove_null() {
    int ret = vec_remove(NULL, 0);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_remove_out_of_bounds() {
    int ret = vec_remove(&vec, 0);
    ASSERT_EQ(ERANGE, ret, "Expected condition to hold");
}

void test_remove_first() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 0);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected pointer to be non-NULL");
    ASSERT_EQ(value2, *item, "Expected condition to hold");

    vec_free(&vec);
}

void test_remove_last() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected pointer to be non-NULL");
    ASSERT_EQ(value1, *item, "Expected condition to hold");

    vec_free(&vec);
}

void test_remove_middle() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    int value3 = 126;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);
    vec_push(&vec, &value3);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(2, vec.size, "Expected condition to hold");

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1, "Expected pointer to be non-NULL");
    ASSERT_EQ(value1, *item1, "Expected condition to hold");

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2, "Expected pointer to be non-NULL");
    ASSERT_EQ(value3, *item2, "Expected condition to hold");

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