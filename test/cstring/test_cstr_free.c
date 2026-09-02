#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

void test_cstr_free_null() {
    int ret = cstr_free(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_free_null_data() {
    static cstr_t str_with_null_data = { .data = NULL, .len = 0, .cap = 4 };
    int ret = cstr_free(&str_with_null_data);
    ASSERT_EQ(0, ret, "Expected condition to hold");
}

void test_cstr_free_valid() {
    cstr_t str = {0};
    int ret = cstr_appendn(&str, "Hello", 5);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = cstr_free(&str);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_NULL(str.data, "Expected pointer to be NULL");
}

void run_cstr_free_tests() {
    RUN_TEST(test_cstr_free_null);
    RUN_TEST(test_cstr_free_null_data);
    RUN_TEST(test_cstr_free_valid);
}