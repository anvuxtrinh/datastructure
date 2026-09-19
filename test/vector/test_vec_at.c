#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;

static void setup(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for vec_at tests");
}

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_at returns NULL when the vector pointer is NULL.
 */
static void test_vec_at_null_vec(void)
{
    int *item = (int *)vec_at(NULL, 0);
    ASSERT_NULL(item, "Expected vec_at to return NULL when vector pointer is NULL");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_at returns NULL for indexes outside the current vector size.
 */
static void test_vec_at_out_of_bounds_index(void)
{
    int value = 42;
    int ret = vec_push(&vec, &value);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed before exercising out-of-bounds reads");

    int *item = (int *)vec_at(&vec, 1);
    ASSERT_NULL(item, "Expected vec_at to return NULL for index equal to vector size");

    item = (int *)vec_at(&vec, 100);
    ASSERT_NULL(item, "Expected vec_at to return NULL for a far out-of-bounds index");

    item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item, "Expected valid indexes to remain readable after out-of-bounds queries");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_at returns stored values for valid indexes.
 */
static void test_vec_at_valid_indexes(void)
{
    int value1 = 42;
    int value2 = 84;
    int ret = vec_push(&vec, &value1);
    ASSERT_EQ(0, ret, "Expected first vec_push to succeed for vec_at happy-path validation");

    ret = vec_push(&vec, &value2);
    ASSERT_EQ(0, ret, "Expected second vec_push to succeed for vec_at happy-path validation");

    int *item1 = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(item1, "Expected vec_at to return the first stored element");
    ASSERT_EQ(value1, *item1, "Expected vec_at index 0 to equal the first pushed value");

    int *item2 = (int *)vec_at(&vec, 1);
    ASSERT_NOT_NULL(item2, "Expected vec_at to return the second stored element");
    ASSERT_EQ(value2, *item2, "Expected vec_at index 1 to equal the second pushed value");
}

// = State Transition Tests
// = Error Code Verification Tests
void run_vec_at_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_at_null_vec);
    RUN_TEST(test_vec_at_valid_indexes);
    RUN_TEST(test_vec_at_out_of_bounds_index);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}