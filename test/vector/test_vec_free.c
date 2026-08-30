#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

void test_vec_free_null() {
    int ret = vec_free(NULL);
    ASSERT_EQ(VEC_ERR_NULL, ret, "Expected condition to hold");
}

void test_vec_free_null_data() {
    static Vec vec_with_null_data = { .data = NULL, .esize = sizeof(int), .size = 0, .cap = 4 };
    int ret = vec_free(&vec_with_null_data);
    ASSERT_EQ(VEC_SUCCESS, ret, "Expected condition to hold");
}
void test_vec_free_valid() {
    Vec vec;
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret, "Expected condition to hold");

    ret = vec_free(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret, "Expected condition to hold");
    ASSERT_NULL(vec.data, "Expected pointer to be NULL");
}

void run_vec_free_tests() {
    RUN_TEST(test_vec_free_null);
    RUN_TEST(test_vec_free_null_data);
    RUN_TEST(test_vec_free_valid);
}