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

void test_cstr_remove_null() {
	int ret = cstr_remove(NULL, 0, 1);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_remove_null_data() {
	int ret = cstr_remove(&str, 0, 1);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_remove_out_of_bounds() {
	int ret = cstr_appendn(&str, "abc", 3);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_remove(&str, 3, 1);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

	ret = cstr_remove(&str, 2, 2);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_remove_valid_middle() {
	int ret = cstr_appendn(&str, "abcdef", 6);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_remove(&str, 2, 2);
	ASSERT_EQ(0, ret, "Expected condition to hold");
	ASSERT_EQ(4, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("abef", str.data, "Expected strings to match");
}

void test_cstr_remove_valid_all() {
	int ret = cstr_appendn(&str, "abc", 3);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_remove(&str, 0, 3);
	ASSERT_EQ(0, ret, "Expected condition to hold");
	ASSERT_EQ(0, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("", str.data, "Expected strings to match");
}

void run_cstr_remove_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_remove_null);
	RUN_TEST(test_cstr_remove_null_data);
	RUN_TEST(test_cstr_remove_out_of_bounds);
	RUN_TEST(test_cstr_remove_valid_middle);
	RUN_TEST(test_cstr_remove_valid_all);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
