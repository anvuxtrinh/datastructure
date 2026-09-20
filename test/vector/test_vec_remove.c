#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;
static int freed_strings;

static void *clone_string_elem(const void *elem)
{
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

static void free_string_elem(void *elem)
{
    freed_strings++;
    free(*(char **)elem);
}

static const data_ops_t string_ops = {
    .clone = clone_string_elem,
    .free = free_string_elem,
};

static void setup(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for vec_remove tests");
    freed_strings = 0;
}

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_remove rejects a NULL vector pointer.
 */
static void test_vec_remove_null_vec(void)
{
    int ret = vec_remove(NULL, 0);
    ASSERT_EQ(EINVAL, ret, "Expected vec_remove to return EINVAL for a NULL vector pointer");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_remove rejects indexes beyond the current vector size.
 */
static void test_vec_remove_out_of_bounds_index(void)
{
    int ret = vec_remove(&vec, 0);
    ASSERT_EQ(ERANGE, ret, "Expected vec_remove to return ERANGE for an empty vector index");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_remove deletes the first element and shifts the remaining value left.
 */
static void test_vec_remove_first_element(void)
{
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed before removing the first element");

    int value1 = 42;
    int value2 = 84;
    ASSERT_EQ(0, vec_push(&vec, &value1), "Expected first vec_push to succeed before removing index 0");
    ASSERT_EQ(0, vec_push(&vec, &value2), "Expected second vec_push to succeed before removing index 0");

    ret = vec_remove(&vec, 0);
    ASSERT_EQ(0, ret, "Expected vec_remove to succeed when removing the first element");
    ASSERT_EQ(1, (int)vec.size, "Expected vec_remove to reduce size after deleting the first element");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected vec_at to return the shifted element after removing index 0");
    ASSERT_EQ(value2, *item, "Expected the second value to move into index 0 after removal");

    vec_deinit(&vec);
}

/**
 * @brief Verify vec_remove deletes the last element without affecting earlier values.
 */
static void test_vec_remove_last_element(void)
{
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed before removing the last element");

    int value1 = 42;
    int value2 = 84;
    ASSERT_EQ(0, vec_push(&vec, &value1), "Expected first vec_push to succeed before removing the last element");
    ASSERT_EQ(0, vec_push(&vec, &value2), "Expected second vec_push to succeed before removing the last element");

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected vec_remove to succeed when removing the last element");
    ASSERT_EQ(1, (int)vec.size, "Expected vec_remove to reduce size after deleting the last element");

    int *item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected vec_at to keep the first element accessible after removing the last element");
    ASSERT_EQ(value1, *item, "Expected the first value to remain unchanged after removing the last element");

    vec_deinit(&vec);
}

/**
 * @brief Verify vec_remove deletes a middle element and compacts the tail correctly.
 */
static void test_vec_remove_middle_element(void)
{
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed before removing a middle element");

    int value1 = 42;
    int value2 = 84;
    int value3 = 126;
    ASSERT_EQ(0, vec_push(&vec, &value1), "Expected first vec_push to succeed before middle removal");
    ASSERT_EQ(0, vec_push(&vec, &value2), "Expected second vec_push to succeed before middle removal");
    ASSERT_EQ(0, vec_push(&vec, &value3), "Expected third vec_push to succeed before middle removal");

    ret = vec_remove(&vec, 1);
    ASSERT_EQ(0, ret, "Expected vec_remove to succeed when removing a middle element");
    ASSERT_EQ(2, (int)vec.size, "Expected vec_remove to reduce size after deleting a middle element");

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1, "Expected vec_at to keep the first element accessible after middle removal");
    ASSERT_EQ(value1, *item1, "Expected the first element to remain unchanged after middle removal");

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2, "Expected vec_at to return the shifted tail element after middle removal");
    ASSERT_EQ(value3, *item2, "Expected the third element to shift into index 1 after middle removal");

    vec_deinit(&vec);
}

// = State Transition Tests
/**
 * @brief Verify vec_remove frees the removed owned element before compacting later elements.
 */
static void test_vec_remove_owned_middle_element_calls_free_cb(void)
{
    vec_t string_vec;
    char *first = "alpha";
    char *second = "beta";
    char *third = "gamma";
    int ret = vec_init(&string_vec, sizeof(char *), &string_ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for owned string elements");

    ret = vec_push(&string_vec, &first);
    ASSERT_EQ(0, ret, "Expected first vec_push to succeed for owned string elements");
    ret = vec_push(&string_vec, &second);
    ASSERT_EQ(0, ret, "Expected second vec_push to succeed for owned string elements");
    ret = vec_push(&string_vec, &third);
    ASSERT_EQ(0, ret, "Expected third vec_push to succeed for owned string elements");

    ret = vec_remove(&string_vec, 1);
    ASSERT_EQ(0, ret, "Expected vec_remove to succeed for the owned middle element");
    ASSERT_EQ(1, freed_strings, "Expected free_cb to run once for the removed owned element");
    ASSERT_EQ(2, (int)string_vec.size, "Expected vec_remove to reduce size after deleting the owned middle element");

    char **first_remaining = (char **)vec_at(&string_vec, 0);
    ASSERT_NOT_NULL(first_remaining, "Expected the first owned string to remain accessible after middle removal");
    ASSERT_STR_EQ("alpha", *first_remaining, "Expected the first owned string to remain unchanged after middle removal");

    char **second_remaining = (char **)vec_at(&string_vec, 1);
    ASSERT_NOT_NULL(second_remaining, "Expected the tail owned string to shift into the removed index");
    ASSERT_STR_EQ("gamma", *second_remaining, "Expected the third owned string to move into index 1 after middle removal");

    ret = vec_deinit(&string_vec);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed after removing an owned middle element");
    ASSERT_EQ(3, freed_strings, "Expected free_cb to run for the removed element and the remaining owned elements by teardown time");
}

// = Error Code Verification Tests
void run_vec_remove_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_remove_null_vec);
    RUN_TEST(test_vec_remove_out_of_bounds_index);
    RUN_TEST(test_vec_remove_first_element);
    RUN_TEST(test_vec_remove_last_element);
    RUN_TEST(test_vec_remove_middle_element);
    RUN_TEST(test_vec_remove_owned_middle_element_calls_free_cb);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}