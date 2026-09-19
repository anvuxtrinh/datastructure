#include <stdio.h>
#include <errno.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;

static void setup(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for shrink_to_fit tests");
}

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_shrink_to_fit rejects a NULL vector pointer.
 */
static void test_vec_shrink_to_fit_null_vec(void)
{
    int ret = vec_shrink_to_fit(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_shrink_to_fit to return EINVAL for a NULL vector pointer");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_shrink_to_fit frees the data buffer for an empty vector.
 */
static void test_vec_shrink_to_fit_empty_vector(void)
{
    int ret = 0;

    for(int i = 0; i < 5; i++) {
        ret = vec_push(&vec, &i);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed while preparing an empty shrink_to_fit case");
    }

    ASSERT_EQ(8, (int)vec.cap, "Expected capacity to grow to eight before clearing the vector");

    ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed before shrinking an empty vector");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_clear to leave the vector empty before shrink_to_fit");

    ret = vec_shrink_to_fit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_shrink_to_fit to succeed for an empty vector");
    ASSERT_NULL(vec.data, "Expected vec_shrink_to_fit to free the data buffer for an empty vector");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_shrink_to_fit to reset capacity to zero for an empty vector");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_shrink_to_fit shrinks capacity to exactly match size for a populated vector.
 */
static void test_vec_shrink_to_fit_non_empty_vector(void)
{
    for(int i = 0; i < 6; i++) {
        int ret = vec_push(&vec, &i);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed while preparing a populated shrink_to_fit case");
    }

    ASSERT_EQ(6, (int)vec.size, "Expected six elements before shrinking a populated vector");
    ASSERT_TRUE(vec.cap >= vec.size, "Expected capacity to be at least the current size before shrinking");

    int ret = vec_shrink_to_fit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_shrink_to_fit to succeed for a populated vector");
    ASSERT_EQ((int)vec.size, (int)vec.cap, "Expected vec_shrink_to_fit to reduce capacity to match size exactly");

    for(int i = 0; i < 6; i++) {
        int *item = (int *)vec_at(&vec, (size_t)i);
        ASSERT_NOT_NULL(item, "Expected stored elements to remain readable after shrink_to_fit");
        ASSERT_EQ(i, *item, "Expected shrink_to_fit to preserve every stored element value");
    }
}

// = State Transition Tests
/**
 * @brief Verify repeated removals trigger automatic capacity shrink without corrupting state.
 */
static void test_vec_remove_auto_shrink_after_removals(void)
{
    for(int i = 0; i < 16; i++) {
        int ret = vec_push(&vec, &i);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed while preparing auto-shrink removal scenario");
    }

    ASSERT_EQ(16, (int)vec.size, "Expected sixteen elements before removal-driven shrinking");
    ASSERT_EQ(16, (int)vec.cap, "Expected capacity to grow to sixteen before removal-driven shrinking");

    for(int i = 0; i < 12; i++) {
        int ret = vec_remove(&vec, vec.size - 1);
        ASSERT_EQ(0, ret, "Expected vec_remove to succeed while shrinking from sixteen elements");
    }

    ASSERT_EQ(4, (int)vec.size, "Expected size to drop to four after removing twelve elements");
    ASSERT_EQ(8, (int)vec.cap, "Expected capacity to shrink to eight after crossing the shrink threshold");

    for(int i = 0; i < 2; i++) {
        int ret = vec_remove(&vec, vec.size - 1);
        ASSERT_EQ(0, ret, "Expected vec_remove to succeed while shrinking from four elements");
    }

    ASSERT_EQ(2, (int)vec.size, "Expected size to drop to two after removing two more elements");
    ASSERT_EQ(4, (int)vec.cap, "Expected capacity to shrink back to the minimum allocation size");
}

// = Error Code Verification Tests
void run_vec_shrink_to_fit_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_shrink_to_fit_null_vec);
    RUN_TEST(test_vec_shrink_to_fit_empty_vector);
    RUN_TEST(test_vec_shrink_to_fit_non_empty_vector);
    RUN_TEST(test_vec_remove_auto_shrink_after_removals);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}