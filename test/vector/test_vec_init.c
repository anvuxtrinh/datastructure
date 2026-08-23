#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static struct Vec vec;

static void teardown() {
    vec_free(&vec);
}

void test_vec_init_zero_size() {
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(0, vec.size);
    ASSERT_EQ(4, vec.cap);
    ASSERT_EQ(sizeof(int), vec.esize);
}

void test_vec_init_null() {
    int ret = vec_init(NULL, 0, sizeof(int));
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

void test_vec_init_zero_esize() {
    int ret = vec_init(&vec, 0, 0);
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

void test_vec_init_with_size() {
    int ret = vec_init(&vec, 10, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(10, vec.size);
    ASSERT_EQ(4, vec.cap);
    ASSERT_EQ(sizeof(int), vec.esize);
}

void test_vec_init_with_struct() {
    int ret = vec_init(&vec, 5, sizeof(double));
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(5, vec.size);
    ASSERT_EQ(4, vec.cap);
    ASSERT_EQ(sizeof(double), vec.esize);
}

void run_vec_init_tests() {
    SET_SETUP(NULL);
    SET_TEARDOWN(teardown);
    RUN_TEST(test_vec_init_zero_size);
    RUN_TEST(test_vec_init_null);
    RUN_TEST(test_vec_init_zero_esize);
    RUN_TEST(test_vec_init_with_size);
    RUN_TEST(test_vec_init_with_struct);
    SET_TEARDOWN(NULL);
}