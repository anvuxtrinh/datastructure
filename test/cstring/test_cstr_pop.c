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

void test_cstr_pop_null() {
	int ret = cstr_pop(NULL);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_pop_null_data() {
	int ret = cstr_pop(&str);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_pop_empty() {
	int ret = cstr_appendn(&str, "A", 1);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_clear(&str);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_pop(&str);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_pop_valid() {
	int ret = cstr_appendn(&str, "Hello", 5);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_pop(&str);
	ASSERT_EQ(0, ret, "Expected condition to hold");
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
