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

void test_cstr_reverse_null() {
	int ret = cstr_reverse(NULL);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_reverse_null_data() {
	int ret = cstr_reverse(&str);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_reverse_valid_even() {
	int ret = cstr_appendn(&str, "abcd", 4);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_reverse(&str);
	ASSERT_EQ(CSTR_SUCCESS, ret);
	ASSERT_STR_EQ("dcba", str.data);
}

void test_cstr_reverse_valid_odd() {
	int ret = cstr_appendn(&str, "abcde", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_reverse(&str);
	ASSERT_EQ(CSTR_SUCCESS, ret);
	ASSERT_STR_EQ("edcba", str.data);
}

void run_cstr_reverse_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_reverse_null);
	RUN_TEST(test_cstr_reverse_null_data);
	RUN_TEST(test_cstr_reverse_valid_even);
	RUN_TEST(test_cstr_reverse_valid_odd);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
