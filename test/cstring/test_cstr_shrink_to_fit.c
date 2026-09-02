#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static cstr_t str;

static void setup() {
    str = (cstr_t){0};
}

static void teardown() {
    cstr_free(&str);
}

void test_cstr_shrink_to_fit_null() {
    int ret = cstr_shrink_to_fit(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_shrink_to_fit_null_data() {
    int ret = cstr_shrink_to_fit(&str);
    ASSERT_EQ(0, ret, "Expected condition to hold");
}

void test_cstr_shrink_to_fit_empty() {
    int ret = cstr_appendn(&str, "A", 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = cstr_clear(&str);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = cstr_shrink_to_fit(&str);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_NULL(str.data, "Expected pointer to be NULL");
    ASSERT_EQ(0, str.cap, "Expected condition to hold");
}

void test_cstr_shrink_to_fit_non_empty() {
    int ret = cstr_appendn(&str, "hello", 5);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ASSERT_TRUE(str.cap > str.len + 1, "Expected condition to be true");

    ret = cstr_shrink_to_fit(&str);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(5, str.len, "Expected condition to hold");
    ASSERT_EQ(6, str.cap, "Expected condition to hold");
    ASSERT_STR_EQ("hello", str.data, "Expected strings to match");
}

void run_cstr_shrink_to_fit_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_cstr_shrink_to_fit_null);
    RUN_TEST(test_cstr_shrink_to_fit_null_data);
    RUN_TEST(test_cstr_shrink_to_fit_empty);
    RUN_TEST(test_cstr_shrink_to_fit_non_empty);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
