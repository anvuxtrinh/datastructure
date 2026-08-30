#include <stdio.h>
#include <stdint.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static Cstr str;

static void setup() {
	str = (Cstr){0};
}

static void teardown() {
	cstr_free(&str);
}

void test_cstr_copy_null() {
	int ret = cstr_copy(NULL, "abc", 3);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");

	ret = cstr_copy(&str, NULL, 3);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");
}

void test_cstr_copy_zero_n() {
	int ret = cstr_copy(&str, "abc", 0);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(0, str.len, "Expected condition to hold");
	ASSERT_NULL(str.data, "Expected pointer to be NULL");
}

void test_cstr_copy_out_of_bounds() {
	int ret = cstr_copy(&str, "abc", SIZE_MAX);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret, "Expected condition to hold");
}

void test_cstr_copy_valid() {
	int ret = cstr_copy(&str, "Hello", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(5, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("Hello", str.data, "Expected strings to match");
}

void test_cstr_copy_overwrite() {
	int ret = cstr_copy(&str, "Hello world", 11);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");

	ret = cstr_copy(&str, "Hi", 2);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(2, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("Hi", str.data, "Expected strings to match");
}

void run_cstr_copy_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_copy_null);
	RUN_TEST(test_cstr_copy_zero_n);
	RUN_TEST(test_cstr_copy_out_of_bounds);
	RUN_TEST(test_cstr_copy_valid);
	RUN_TEST(test_cstr_copy_overwrite);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
