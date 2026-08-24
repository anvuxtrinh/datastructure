#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static Vec vec;

void setup() {
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
}

void teardown() {
    vec_free(&vec);
}

void test_vec_at_null() {
    int *item = (int *)vec_at(NULL, 0);
    ASSERT_NULL(item);
}

void test_vec_at_out_of_bounds() {
    int value = 42;
    vec_push(&vec, &value);

    int *item = (int *)vec_at(&vec, 1);
    ASSERT_NULL(item);

    item = (int *)vec_at(&vec, 100);
    ASSERT_NULL(item);

    item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item);
}

void test_vec_at_valid() {
    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1);
    ASSERT_EQ(value1, *item1);

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2);
    ASSERT_EQ(value2, *item2);
}

void run_vec_at_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);
    
    RUN_TEST(test_vec_at_null);
    RUN_TEST(test_vec_at_valid);
    RUN_TEST(test_vec_at_out_of_bounds);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}