#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static struct Vec vec;

static void setup() {
    int ret = vec_init(&vec, 0, sizeof(int));
    ASSERT_EQ(VEC_SUCCESS, ret);
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

void run_vec_clear_tests(void) {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_clear_null);
    RUN_TEST(test_vec_clear_empty);
    RUN_TEST(test_vec_clear_with_data);
    RUN_TEST(test_vec_clear_then_push_again);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}