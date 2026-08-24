#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static Vec vec;
static int free_cb_calls;

static void count_free_cb(void *elem) {
    (void)elem;
    free_cb_calls++;
}

static void setup() {
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
    free_cb_calls = 0;
}

static void teardown() {
    vec_free(&vec);
}

void test_vec_clear_null() {
    int ret = vec_clear(NULL);
    ASSERT_EQ(VEC_ERR_NULL, ret);
}

void test_vec_clear_empty() {
    int ret = vec_clear(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(0, vec.size);
}

void test_vec_clear_with_data() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size);

    int ret = vec_clear(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(0, vec.size);
}

void test_vec_clear_then_push_again() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size);

    int ret = vec_clear(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(0, vec.size);

    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size);
}

void test_vec_clear_calls_free_cb_for_each_element() {
    int a = 1;
    int b = 2;
    int c = 3;
    vec_push(&vec, &a);
    vec_push(&vec, &b);
    vec_push(&vec, &c);

    vec_set_free_cb(&vec, count_free_cb);
    int ret = vec_clear(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(0, vec.size);
    ASSERT_EQ(3, free_cb_calls);
}

void test_vec_free_calls_free_cb_via_clear() {
    int a = 1;
    int b = 2;
    vec_push(&vec, &a);
    vec_push(&vec, &b);

    vec_set_free_cb(&vec, count_free_cb);
    int ret = vec_free(&vec);
    ASSERT_EQ(VEC_SUCCESS, ret);
    ASSERT_EQ(2, free_cb_calls);
    ASSERT_NULL(vec.data);
    ASSERT_EQ(0, vec.size);
    ASSERT_EQ(0, vec.cap);
}

void test_vec_set_free_cb_null_safe() {
    vec_set_free_cb(NULL, count_free_cb);
    ASSERT_TRUE(1);
}

void run_vec_clear_tests(void) {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_clear_null);
    RUN_TEST(test_vec_clear_empty);
    RUN_TEST(test_vec_clear_with_data);
    RUN_TEST(test_vec_clear_then_push_again);
    RUN_TEST(test_vec_clear_calls_free_cb_for_each_element);
    RUN_TEST(test_vec_free_calls_free_cb_via_clear);
    RUN_TEST(test_vec_set_free_cb_null_safe);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}