#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

void test_cstr_free_null() {
    int ret = cstr_free(NULL);
    ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_free_null_data() {
    static Cstr str_with_null_data = { .data = NULL, .len = 0, .cap = 4 };
    int ret = cstr_free(&str_with_null_data);
    ASSERT_EQ(CSTR_SUCCESS, ret);
}

void test_cstr_free_valid() {
    Cstr str = {0};
    int ret = cstr_appendn(&str, "Hello", 5);
    ASSERT_EQ(CSTR_SUCCESS, ret);

    ret = cstr_free(&str);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_NULL(str.data);
}

void run_cstr_free_tests() {
    RUN_TEST(test_cstr_free_null);
    RUN_TEST(test_cstr_free_null_data);
    RUN_TEST(test_cstr_free_valid);
}