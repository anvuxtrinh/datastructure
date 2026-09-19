#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_init rejects a NULL vector pointer.
 */
static void test_vec_init_null_vec(void)
{
    int ret = vec_init(NULL, sizeof(int), NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_init to return EINVAL when vector pointer is NULL");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_init creates an empty vector with zero size and capacity.
 */
static void test_vec_init_empty_state(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for a valid integer element size");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_init to initialize size to zero");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_init to initialize capacity to zero");
    ASSERT_EQ((int)sizeof(int), (int)vec.esize, "Expected vec_init to store the requested integer element size");
}

// = Error Code Verification Tests
/**
 * @brief Verify vec_init rejects a zero element size.
 */
static void test_vec_init_zero_element_size(void)
{
    int ret = vec_init(&vec, 0, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_init to return EINVAL when element size is zero");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_init stores integer element metadata correctly.
 */
static void test_vec_init_with_integer_element_size(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for integer elements");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_init to leave size at zero for integer elements");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_init to leave capacity at zero for integer elements");
    ASSERT_EQ((int)sizeof(int), (int)vec.esize, "Expected vec_init to store integer element size metadata");
}

/**
 * @brief Verify vec_init stores struct-sized element metadata correctly.
 */
static void test_vec_init_with_struct_element_size(void)
{
    int ret = vec_init(&vec, sizeof(double), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for struct-sized elements");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_init to initialize size to zero for struct-sized elements");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_init to initialize capacity to zero for struct-sized elements");
    ASSERT_EQ((int)sizeof(double), (int)vec.esize, "Expected vec_init to store the struct-sized element width");
}

/**
 * @brief Verify vec_init stores the provided operations table.
 */
static void test_vec_init_with_ops_table(void)
{
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = NULL,
    };

    int ret = vec_init(&vec, sizeof(int), &ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed when an operations table is provided");
    ASSERT_TRUE(vec.ops == &ops, "Expected vec_init to store the provided operations table pointer");
}

// = State Transition Tests
void run_vec_init_tests(void)
{
    SET_SETUP(NULL);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_init_empty_state);
    RUN_TEST(test_vec_init_null_vec);
    RUN_TEST(test_vec_init_zero_element_size);
    RUN_TEST(test_vec_init_with_integer_element_size);
    RUN_TEST(test_vec_init_with_struct_element_size);
    RUN_TEST(test_vec_init_with_ops_table);

    SET_TEARDOWN(NULL);
}