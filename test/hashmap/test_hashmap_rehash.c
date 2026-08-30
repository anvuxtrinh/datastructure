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

// = Boundary Condition Tests
/**
 * @brief Verify rehash handles a nearly-full table without losing existing keys.
 *
 * Once the load threshold is reached, the map should expand and still access each
 * previously stored key.
 */
static void test_hashmap_rehash_after_threshold(void)
{
    const char *keys[] = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta"};
    int values[] = {10, 20, 30, 40, 50, 60, 70};
    const size_t count = sizeof(keys) / sizeof(keys[0]);

    for (size_t i = 0; i < count; i++) {
        int ret = hashmap_put(&map, (char *)keys[i], (int)strlen(keys[i]), &values[i]);
        ASSERT_EQ(0, ret, "Expected insertion to succeed while filling the map");
    }

    ASSERT_TRUE(map.cap > 8, "Expected map capacity to increase after reaching the threshold");

    for (size_t i = 0; i < count; i++) {
        int *stored = (int *)hashmap_get(&map, (char *)keys[i], (int)strlen(keys[i]));
        ASSERT_NOT_NULL(stored, "Expected values to remain readable after rehashing");
        ASSERT_EQ(values[i], *stored, "Expected values to survive rehashing without corruption");
    }
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify rehash on a small map leaves the map ready for continued use.
 *
 * A structure that is rehashed after growth should accept new insertions and keep
 * old data accessible.
 */
static void test_hashmap_rehash_keeps_map_usable(void)
{
    char first_key[] = "alpha";
    char second_key[] = "beta";
    int first_value = 10;
    int second_value = 20;

    int ret = hashmap_put(&map, first_key, strlen(first_key), &first_value);
    ASSERT_EQ(0, ret, "Expected first insertion to succeed");

    ret = hashmap_put(&map, second_key, strlen(second_key), &second_value);
    ASSERT_EQ(0, ret, "Expected second insertion to succeed");

    int *stored_first = (int *)hashmap_get(&map, first_key, strlen(first_key));
    int *stored_second = (int *)hashmap_get(&map, second_key, strlen(second_key));
    ASSERT_NOT_NULL(stored_first, "Expected first key to remain accessible after rehashing");
    ASSERT_NOT_NULL(stored_second, "Expected second key to remain accessible after rehashing");
    ASSERT_EQ(first_value, *stored_first, "Expected original value to remain correct");
    ASSERT_EQ(second_value, *stored_second, "Expected second value to remain correct");
}

// = State Transition Tests
/**
 * @brief Verify the map survives a growth cycle followed by a later insert.
 *
 * Rehashing should not prevent future insertions or cause missing entries after a
 * growth event.
 */
static void test_hashmap_rehash_then_insert_new_key(void)
{
    char old_key[] = "alpha";
    char new_key[] = "gamma";
    int old_value = 10;
    int new_value = 30;

    int ret = hashmap_put(&map, old_key, strlen(old_key), &old_value);
    ASSERT_EQ(0, ret, "Expected initial insertion to succeed");

    ret = hashmap_put(&map, new_key, strlen(new_key), &new_value);
    ASSERT_EQ(0, ret, "Expected insertion after rehash to succeed");

    int *stored_new = (int *)hashmap_get(&map, new_key, strlen(new_key));
    ASSERT_NOT_NULL(stored_new, "Expected newly inserted key to be accessible after a growth cycle");
    ASSERT_EQ(new_value, *stored_new, "Expected new value to be stored after rehashing");
}

// = Error Code Verification Tests

void run_hashmap_rehash_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_rehash_after_threshold);
    RUN_TEST(test_hashmap_rehash_keeps_map_usable);
    RUN_TEST(test_hashmap_rehash_then_insert_new_key);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
