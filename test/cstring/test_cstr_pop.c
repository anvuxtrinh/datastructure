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

void test_cstr_pop_null() {
	int ret = cstr_pop(NULL);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");
}

void test_cstr_pop_null_data() {
	int ret = cstr_pop(&str);
	ASSERT_EQ(CSTR_ERR_NULL, ret, "Expected condition to hold");
}

void test_cstr_pop_empty() {
	int ret = cstr_appendn(&str, "A", 1);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");

	ret = cstr_clear(&str);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");

	ret = cstr_pop(&str);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret, "Expected condition to hold");
}

void test_cstr_pop_valid() {
	int ret = cstr_appendn(&str, "Hello", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");

	ret = cstr_pop(&str);
	ASSERT_EQ(CSTR_SUCCESS, ret, "Expected condition to hold");
	ASSERT_EQ(4, str.len, "Expected condition to hold");
	ASSERT_STR_EQ("Hell", str.data, "Expected strings to match");
}

void run_cstr_pop_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_pop_null);
	RUN_TEST(test_cstr_pop_null_data);
	RUN_TEST(test_cstr_pop_empty);
	RUN_TEST(test_cstr_pop_valid);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
