#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;

static void setup(void) {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
}

static void teardown(void) {
    vec_free(&vec);
}

void test_vec_shrink_to_fit_null(void) {
    int ret = vec_shrink_to_fit(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_shrink_to_fit_empty_vector(void) {
    int value = 7;
    int ret = vec_push(&vec, &value);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_TRUE(vec.cap > 0, "Expected condition to be true");

    ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, (int)vec.size, "Expected condition to hold");

    ret = vec_shrink_to_fit(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_NULL(vec.data, "Expected pointer to be NULL");
    ASSERT_EQ(0, (int)vec.cap, "Expected condition to hold");
}

void test_vec_shrink_to_fit_non_empty_vector(void) {
    for(int i = 0; i < 6; i++) {
        int ret = vec_push(&vec, &i);
        ASSERT_EQ(0, ret, "Expected condition to hold");
    }

    ASSERT_EQ(6, (int)vec.size, "Expected condition to hold");
    ASSERT_TRUE(vec.cap >= vec.size, "Expected condition to be true");

    int ret = vec_shrink_to_fit(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ((int)vec.size, (int)vec.cap, "Expected condition to hold");

    for(int i = 0; i < 6; i++) {
        int *item = (int *)vec_at(&vec, (size_t)i);
        ASSERT_NOT_NULL(item, "Expected pointer to be non-NULL");
        ASSERT_EQ(i, *item, "Expected condition to hold");
    }
}

void test_vec_remove_auto_shrink(void) {
    for(int i = 0; i < 16; i++) {
        int ret = vec_push(&vec, &i);
        ASSERT_EQ(0, ret, "Expected condition to hold");
    }

    ASSERT_EQ(16, (int)vec.size, "Expected condition to hold");
    ASSERT_EQ(16, (int)vec.cap, "Expected condition to hold");

    for(int i = 0; i < 12; i++) {
        int ret = vec_remove(&vec, vec.size - 1);
        ASSERT_EQ(0, ret, "Expected condition to hold");
    }

    ASSERT_EQ(4, (int)vec.size, "Expected condition to hold");
    ASSERT_EQ(8, (int)vec.cap, "Expected condition to hold");

    for(int i = 0; i < 2; i++) {
        int ret = vec_remove(&vec, vec.size - 1);
        ASSERT_EQ(0, ret, "Expected condition to hold");
    }

    ASSERT_EQ(2, (int)vec.size, "Expected condition to hold");
    ASSERT_EQ(4, (int)vec.cap, "Expected condition to hold");
}

void run_vec_shrink_to_fit_tests(void) {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_shrink_to_fit_null);
    RUN_TEST(test_vec_shrink_to_fit_empty_vector);
    RUN_TEST(test_vec_shrink_to_fit_non_empty_vector);
    RUN_TEST(test_vec_remove_auto_shrink);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}