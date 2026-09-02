#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;
static int free_cb_calls;

static void count_free_cb(void *elem) {
    (void)elem;
    free_cb_calls++;
}

static void setup() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    free_cb_calls = 0;
}

static void teardown() {
    vec_free(&vec);
}

void test_vec_clear_null() {
    int ret = vec_clear(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_clear_empty() {
    int ret = vec_clear(&vec);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_clear_with_data() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
}

void test_vec_clear_then_push_again() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");

    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");
}

void test_vec_clear_calls_free_cb_for_each_element() {
    vec_free(&vec);
    int init_ret = vec_init(&vec, sizeof(int), count_free_cb);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    int a = 1;
    int b = 2;
    int c = 3;
    vec_push(&vec, &a);
    vec_push(&vec, &b);
    vec_push(&vec, &c);

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(3, free_cb_calls, "Expected condition to hold");
}

void test_vec_free_calls_free_cb_via_clear() {
    vec_free(&vec);
    int init_ret = vec_init(&vec, sizeof(int), count_free_cb);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    int a = 1;
    int b = 2;
    vec_push(&vec, &a);
    vec_push(&vec, &b);

    int ret = vec_free(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(2, free_cb_calls, "Expected condition to hold");
    ASSERT_NULL(vec.data, "Expected pointer to be NULL");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
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

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}