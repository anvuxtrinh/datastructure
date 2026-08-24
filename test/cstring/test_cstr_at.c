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

void test_cstr_at_null() {
	char ch = cstr_at(NULL, 0);
	ASSERT_EQ('\0', ch);
}

void test_cstr_at_out_of_bounds() {
	int ret = cstr_appendn(&str, "Hi", 2);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	char ch = cstr_at(&str, 2);
	ASSERT_EQ('\0', ch);

	ch = cstr_at(&str, 100);
	ASSERT_EQ('\0', ch);
}

void test_cstr_at_valid() {
	int ret = cstr_appendn(&str, "World", 5);
	ASSERT_EQ(CSTR_SUCCESS, ret);

	char ch0 = cstr_at(&str, 0);
	char ch4 = cstr_at(&str, 4);
	ASSERT_EQ('W', ch0);
	ASSERT_EQ('d', ch4);
}

void run_cstr_at_tests() {
	SET_SETUP(setup);
	SET_TEARDOWN(teardown);

	RUN_TEST(test_cstr_at_null);
	RUN_TEST(test_cstr_at_out_of_bounds);
	RUN_TEST(test_cstr_at_valid);

	SET_SETUP(NULL);
	SET_TEARDOWN(NULL);
}
