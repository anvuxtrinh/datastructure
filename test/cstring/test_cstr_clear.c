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

void test_cstr_clear_null() {
	int ret = cstr_clear(NULL);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");
}

void test_cstr_clear_null_data() {
	int ret = cstr_clear(&str);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");
}

void test_cstr_clear_with_data() {
	int ret = cstr_appendn(&str, "Hello", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(5, str.len, "Expected condition to hold");

	ret = cstr_clear(&str);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(0, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("", str.data, "Expected strings to match");
}

void run_cstr_clear_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_clear_null);
	RUN_TEST(test_cstr_clear_null_data);
	RUN_TEST(test_cstr_clear_with_data);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
