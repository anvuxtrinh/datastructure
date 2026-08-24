#include <stdio.h>
#include "framework/framework.h"
#include "../../inc/cstr.h"

static Cstr str;
static Cstr out;

static void setup() {
	str = (Cstr){0};
	out = (Cstr){0};
}

static void teardown() {
	cstr_free(&str);
	cstr_free(&out);
}

void test_cstr_substr_null() {
	int ret = cstr_substr(NULL, 0, 1, &out);
	ASSERT_EQ(CSTR_ERR_NULL, ret);

	ret = cstr_substr(&str, 0, 1, NULL);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_substr_null_data() {
	int ret = cstr_substr(&str, 0, 1, &out);
	ASSERT_EQ(CSTR_ERR_NULL, ret);
}

void test_cstr_substr_out_of_bounds() {
	int ret = cstr_appendn(&str, "hello", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_substr(&str, 5, 5, &out);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);

	ret = cstr_substr(&str, 1, 6, &out);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);

	ret = cstr_substr(&str, 3, 2, &out);
	ASSERT_EQ(CSTR_ERR_OUT_OF_BOUNDS, ret);
}

void test_cstr_substr_valid() {
	int ret = cstr_appendn(&str, "hello", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	ret = cstr_substr(&str, 1, 4, &out);
	ASSERT_EQ(CSTR_SUCCESS, ret);
	ASSERT_EQ(3, out.len);
	ASSERT_STR_EQ("ell", out.data);
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
