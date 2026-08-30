#include <errno.h>
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
 * @brief Verify hashmap_get handles a NULL map pointer safely.
 *
 * Querying with a null map is invalid and should not crash. The public API should
 * treat it as an absent lookup and return NULL.
 */
static void test_hashmap_get_null_map(void)
{
    char key[] = "alpha";
    int *value = (int *)hashmap_get(NULL, key, strlen(key));
    ASSERT_NULL(value, "Expected NULL when hashmap pointer is NULL");
}

/**
 * @brief Verify hashmap_get returns NULL for a missing key.
 *
 * A lookup for a key that was never inserted should be treated as a miss.
 */
static void test_hashmap_get_missing_key(void)
{
    char key[] = "missing";
    int *value = (int *)hashmap_get(&map, key, strlen(key));
    ASSERT_NULL(value, "Expected NULL when the key is not present in the map");
}

// = Boundary Condition Tests
/**
 * @brief Verify hashmap_get handles empty buckets and zero-length keys.
 *
 * Empty or invalid lookup values should not read out of range or return stale
 * values.
 */
static void test_hashmap_get_zero_length_key(void)
{
    int *value = (int *)hashmap_get(&map, "alpha", 0);
    ASSERT_NULL(value, "Expected NULL for a zero-length lookup key");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify hashmap_get returns the stored value for a valid key.
 *
 * Once a key and value are stored, a later lookup by the same bytes and size must
 * return the exact stored object.
 */
static void test_hashmap_get_valid_key(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected insertion to succeed before lookup");

    int *stored = (int *)hashmap_get(&map, key, strlen(key));
    ASSERT_NOT_NULL(stored, "Expected retrieved value to be non-NULL");
    ASSERT_EQ(value, *stored, "Expected the retrieved value to match the stored value");
}

// = State Transition Tests
/**
 * @brief Verify hashmap_get still works after a key is updated.
 *
 * A later insertion with the same key should replace the stored value and the
 * retrieval should reflect the new value.
 */
static void test_hashmap_get_after_update(void)
{
    char key[] = "alpha";
    int first_value = 42;
    int second_value = 99;

    int ret = hashmap_put(&map, key, strlen(key), &first_value);
    ASSERT_EQ(0, ret, "Expected first insertion to succeed");

    ret = hashmap_put(&map, key, strlen(key), &second_value);
    ASSERT_EQ(0, ret, "Expected overwrite to succeed");

    int *stored = (int *)hashmap_get(&map, key, strlen(key));
    ASSERT_NOT_NULL(stored, "Expected updated value to be accessible");
    ASSERT_EQ(second_value, *stored, "Expected lookup to return the newest value");
}

// = Error Code Verification Tests

void run_hashmap_get_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_get_null_map);
    RUN_TEST(test_hashmap_get_missing_key);
    RUN_TEST(test_hashmap_get_zero_length_key);
    RUN_TEST(test_hashmap_get_valid_key);
    RUN_TEST(test_hashmap_get_after_update);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
