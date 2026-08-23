#include <stdio.h>
#include <stdint.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static cstr str;

static void setup() {
}

static void teardown() {
    cstr_free(&str);
}

void test_cstr_appendn_null_self() {
    int ret = cstr_appendn(NULL, "Hello", 5);
    ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_appendn_null_str() {
    int ret = cstr_appendn(&str, NULL, 5);
    ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_appendn_zero_n() {
    int ret = cstr_appendn(&str, "Hello", 0);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_EQ(0, str.len);
    ASSERT_NULL(str.data);
}

void test_cstr_appendn_out_of_bounds() {
    size_t large_n = SIZE_MAX - str.len;
    int ret = cstr_appendn(&str, "Hello", large_n);
    ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);
}

void test_cstr_appendn_newsize_exceeds_cap() {
    int ret = cstr_appendn(&str, "Hello", 5);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_STR_EQ("Hello", str.data);
    ASSERT_EQ(5, str.len);
    ASSERT_TRUE(str.cap >= 6);

    ret = cstr_appendn(&str, " World", 6);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_STR_EQ("Hello World", str.data);
    ASSERT_EQ(11, str.len);
    ASSERT_TRUE(str.cap >= 12);
}

void test_cstr_appendn_within_cap() {
    int ret = cstr_appendn(&str, "Hello", 5);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_STR_EQ("Hello", str.data);
    ASSERT_EQ(5, str.len);
    ASSERT_TRUE(str.cap >= 6);

    ret = cstr_appendn(&str, "!", 1);
    ASSERT_EQ(CSTR_SUCCESS, ret);
    ASSERT_STR_EQ("Hello!", str.data);
    ASSERT_EQ(6, str.len);
    ASSERT_TRUE(str.cap >= 7);
}

void run_cstr_appendn_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);
    RUN_TEST(test_cstr_appendn_null_self);
    RUN_TEST(test_cstr_appendn_null_str);
    RUN_TEST(test_cstr_appendn_zero_n);
    RUN_TEST(test_cstr_appendn_out_of_bounds);
    RUN_TEST(test_cstr_appendn_newsize_exceeds_cap);
    RUN_TEST(test_cstr_appendn_within_cap);
    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}