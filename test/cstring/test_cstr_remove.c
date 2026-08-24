#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static Cstr str;

static void setup() {
	str = (Cstr){0};
}

static void teardown() {
	cstr_free(&str);
}

void test_cstr_remove_null() {
	int ret = cstr_remove(NULL, 0, 1);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_remove_null_data() {
	int ret = cstr_remove(&str, 0, 1);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_remove_out_of_bounds() {
	int ret = cstr_appendn(&str, "abc", 3);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_remove(&str, 3, 1);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);

	ret = cstr_remove(&str, 2, 2);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);
}

void test_cstr_remove_valid_middle() {
	int ret = cstr_appendn(&str, "abcdef", 6);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_remove(&str, 2, 2);
	ASSERT_EQ(CSTR_SUCCESS, ret);
	ASSERT_EQ(4, str.len);
	ASSERT_STR_EQ("abef", str.data);
}

void test_cstr_remove_valid_all() {
	int ret = cstr_appendn(&str, "abc", 3);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_remove(&str, 0, 3);
	ASSERT_EQ(CSTR_SUCCESS, ret);
	ASSERT_EQ(0, str.len);
	ASSERT_STR_EQ("", str.data);
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
