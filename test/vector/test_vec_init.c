#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;

static void teardown() {
    vec_free(&vec);
}

void test_vec_init_zero_size() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
    ASSERT_EQ(sizeof(int), vec.esize, "Expected condition to hold");
}

void test_vec_init_null() {
    int ret = vec_init(NULL, sizeof(int), NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_init_zero_esize() {
    int ret = vec_init(&vec, 0, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_init_with_size() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
    ASSERT_EQ(sizeof(int), vec.esize, "Expected condition to hold");
}

void test_vec_init_with_struct() {
    int ret = vec_init(&vec, sizeof(double), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
    ASSERT_EQ(sizeof(double), vec.esize, "Expected condition to hold");
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