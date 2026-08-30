#include <stdlib.h>
#include <string.h>

#include "framework/framework.h"
#include "../../inc/hashmap.h"

static hashmap_t map;

static void setup(void)
{
    map = (hashmap_t){0};
}

static void teardown(void)
{
    hashmap_free(&map);
}

// = Test Fixtures

// = Null Input Tests
/**
 * @brief Verify hashmap_free handles a valid map and resets internal state.
 *
 * When a non-empty map is freed, every bucket chain must be released and the map
 * state must be reset to a clean default.
 */
static void test_hashmap_free_valid_map(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected insertion to succeed before freeing the map");

    hashmap_free(&map);
    ASSERT_EQ(0, map.size, "Expected size to reset to zero after freeing the map");
    ASSERT_EQ(0, map.cap, "Expected capacity to reset to zero after freeing the map");
    ASSERT_NULL(map.buckets, "Expected bucket table to be released to NULL after freeing the map");
}

// = Boundary Condition Tests
/**
 * @brief Verify hashmap_free is safe on an empty map.
 *
 * An empty map should free cleanly without leaving any dangling bucket memory or
 * stale counters.
 */
static void test_hashmap_free_empty_map(void)
{
    hashmap_free(&map);
    ASSERT_EQ(0, map.size, "Expected empty map size to stay at zero after free");
    ASSERT_EQ(0, map.cap, "Expected empty map capacity to remain zero after free");
    ASSERT_NULL(map.buckets, "Expected empty map bucket table to be NULL after free");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify hashmap_free can be called again after an empty-state reset.
 *
 * A map that has already been cleaned should remain safe to release a second time
 * under the same reset semantics.
 */
static void test_hashmap_free_after_reset(void)
{
    hashmap_free(&map);
    hashmap_free(&map);
    ASSERT_EQ(0, map.size, "Expected size to remain zero after a second free call");
    ASSERT_EQ(0, map.cap, "Expected capacity to remain zero after a second free call");
    ASSERT_NULL(map.buckets, "Expected bucket pointer to remain NULL after a second free call");
}

// = State Transition Tests

// = Error Code Verification Tests

void run_hashmap_free_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_free_valid_map);
    RUN_TEST(test_hashmap_free_empty_map);
    RUN_TEST(test_hashmap_free_after_reset);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
