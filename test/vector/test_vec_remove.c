#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;
static int freed_strings;

static void *clone_string_elem(const void *elem) {
    const char *src = *(const char * const *)elem;
    char *copy = malloc(strlen(src) + 1);
    char **boxed = NULL;

    if(copy == NULL) {
        return NULL;
    }

    strcpy(copy, src);

    boxed = malloc(sizeof(*boxed));
    if(boxed == NULL) {
        free(copy);
        return NULL;
    }

    *boxed = copy;
    return boxed;
}

static void free_string_elem(void *elem) {
    freed_strings++;
    free(*(char **)elem);
}

static const vec_ops_t string_ops = {
    .clone_cb = clone_string_elem,
    .free_cb = free_string_elem,
};

static void setup() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    freed_strings = 0;
}

static void teardown() {
    vec_deinit(&vec);
}

void test_remove_null() {
    int ret = vec_remove(NULL, 0);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_remove_out_of_bounds() {
    int ret = vec_remove(&vec, 0);
    ASSERT_EQ(ERANGE, ret, "Expected condition to hold");
}

void test_remove_first() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 0);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected pointer to be non-NULL");
    ASSERT_EQ(value2, *item, "Expected condition to hold");

    vec_deinit(&vec);
}

void test_remove_last() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected pointer to be non-NULL");
    ASSERT_EQ(value1, *item, "Expected condition to hold");

    vec_deinit(&vec);
}

void test_remove_middle() {
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    int value1 = 42;
    int value2 = 84;
    int value3 = 126;
    vec_push(&vec, &value1);
    vec_push(&vec, &value2);
    vec_push(&vec, &value3);

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(2, vec.size, "Expected condition to hold");

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1, "Expected pointer to be non-NULL");
    ASSERT_EQ(value1, *item1, "Expected condition to hold");

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2, "Expected pointer to be non-NULL");
    ASSERT_EQ(value3, *item2, "Expected condition to hold");

    vec_deinit(&vec);
}

void test_remove_deep_copied_pointer_calls_free_cb() {
    vec_t string_vec;
    char *first = "alpha";
    char *second = "beta";
    char *third = "gamma";
    int ret = vec_init(&string_vec, sizeof(char *), &string_ops);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_push(&string_vec, &first);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ret = vec_push(&string_vec, &second);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ret = vec_push(&string_vec, &third);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_remove(&string_vec, 1);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, freed_strings, "Expected condition to hold");
    ASSERT_EQ(2, string_vec.size, "Expected condition to hold");

    char **first_remaining = (char **)vec_at(&string_vec, 0);
    ASSERT_NOT_NULL(first_remaining, "Expected pointer to be non-NULL");
    ASSERT_STR_EQ("alpha", *first_remaining, "Expected strings to be equal");

    char **second_remaining = (char **)vec_at(&string_vec, 1);
    ASSERT_NOT_NULL(second_remaining, "Expected pointer to be non-NULL");
    ASSERT_STR_EQ("gamma", *second_remaining, "Expected strings to be equal");

    ret = vec_deinit(&string_vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(3, freed_strings, "Expected condition to hold");
}

void run_vec_remove_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_remove_null);
    RUN_TEST(test_remove_out_of_bounds);
    RUN_TEST(test_remove_first);
    RUN_TEST(test_remove_last);
    RUN_TEST(test_remove_middle);
    RUN_TEST(test_remove_deep_copied_pointer_calls_free_cb);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}