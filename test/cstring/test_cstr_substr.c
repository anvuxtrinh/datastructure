#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static cstr_t str;
static cstr_t out;

static void setup() {
	str = (cstr_t){0};
	out = (cstr_t){0};
}

static void teardown() {
	cstr_free(&str);
	cstr_free(&out);
}

void test_cstr_substr_null() {
	int ret = cstr_substr(NULL, 0, 1, &out);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

	ret = cstr_substr(&str, 0, 1, NULL);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_substr_null_data() {
	int ret = cstr_substr(&str, 0, 1, &out);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_substr_out_of_bounds() {
	int ret = cstr_appendn(&str, "hello", 5);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_substr(&str, 5, 5, &out);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

	ret = cstr_substr(&str, 1, 6, &out);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

	ret = cstr_substr(&str, 3, 2, &out);
	ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_cstr_substr_valid() {
	int ret = cstr_appendn(&str, "hello", 5);
	ASSERT_EQ(0, ret, "Expected condition to hold");

	ret = cstr_substr(&str, 1, 4, &out);
	ASSERT_EQ(0, ret, "Expected condition to hold");
	ASSERT_EQ(3, out.len, "Expected condition to hold");
	ASSERT_STR_EQ("ell", out.data, "Expected strings to match");
}

void run_cstr_substr_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_substr_null);
	RUN_TEST(test_cstr_substr_null_data);
	RUN_TEST(test_cstr_substr_out_of_bounds);
	RUN_TEST(test_cstr_substr_valid);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
