#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures

// = Null Input Tests
/**
 * @brief Verify vec_deinit rejects a NULL vector pointer.
 */
static void test_vec_deinit_null_vec(void)
{
    int ret = vec_deinit(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_deinit to return EINVAL for a NULL vector pointer");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_deinit resets size and capacity when data is already NULL.
 */
static void test_vec_deinit_null_data_buffer(void)
{
    static vec_t vec_with_null_data = { .data = NULL, .esize = sizeof(int), .size = 0, .cap = 4, .ops = NULL };
    int ret = vec_deinit(&vec_with_null_data);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed when the data pointer is already NULL");
    ASSERT_EQ(0, (int)vec_with_null_data.size, "Expected vec_deinit to reset size when the data pointer is already NULL");
    ASSERT_EQ(0, (int)vec_with_null_data.cap, "Expected vec_deinit to reset capacity when the data pointer is already NULL");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_deinit releases storage and resets vector state for a valid vector.
 */
static void test_vec_deinit_valid_vector(void)
{
    vec_t vec;
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed before vec_deinit happy-path validation");

    ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed for a valid initialized vector");
    ASSERT_NULL(vec.data, "Expected vec_deinit to set data to NULL");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_deinit to reset size to zero");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_deinit to reset capacity to zero");
}

// = State Transition Tests
// = Error Code Verification Tests
void run_vec_deinit_tests(void)
{
    RUN_TEST(test_vec_deinit_null_vec);
    RUN_TEST(test_vec_deinit_null_data_buffer);
    RUN_TEST(test_vec_deinit_valid_vector);
}