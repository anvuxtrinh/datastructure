#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

void test_vec_free_null() {
    int ret = vec_deinit(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_free_null_data() {
    static vec_t vec_with_null_data = { .data = NULL, .esize = sizeof(int), .size = 0, .cap = 4, .ops = NULL };
    int ret = vec_deinit(&vec_with_null_data);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec_with_null_data.size, "Expected condition to hold");
    ASSERT_EQ(0, vec_with_null_data.cap, "Expected condition to hold");
}

void test_vec_free_valid() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_NULL(vec.data, "Expected pointer to be NULL");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
}

void run_vec_free_tests() {
    RUN_TEST(test_vec_free_null);
    RUN_TEST(test_vec_free_null_data);
    RUN_TEST(test_vec_free_valid);
}