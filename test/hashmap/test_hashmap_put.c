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
 * @brief Verify hashmap_put rejects a NULL map handle and NULL key pointer.
 *
 * The map API must fail fast on invalid input rather than dereferencing a null
 * pointer or reading from an uninitialized bucket table.
 */
static void test_hashmap_put_null_map(void)
{
    int value = 42;
    char key[] = "alpha";

    int ret = hashmap_put(NULL, key, strlen(key), &value);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when hashmap pointer is NULL");

    ret = hashmap_put(&map, NULL, strlen(key), &value);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when key pointer is NULL");
}

// = Boundary Condition Tests
/**
 * @brief Verify hashmap_put performs lazy allocation of the bucket table.
 */
static void test_hashmap_put_lazy_allocation(void)
{
    hashmap_t empty_map = {0, 0, NULL};
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&empty_map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected successful lazy allocation of the bucket table");

    ret = hashmap_put(&empty_map, key, 0, &value);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when key size is zero");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify hashmap_put stores a valid value and updates the bucket count.
 *
 * A normal insertion should succeed and add one logical entry to the map.
 */
static void test_hashmap_put_valid_insert(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected successful insertion of a valid key-value pair");
    ASSERT_EQ(1, map.size, "Expected map size to increase by one after insertion");

    int *stored = (int *)hashmap_get(&map, key, strlen(key));
    ASSERT_NOT_NULL(stored, "Expected stored value to be retrievable");
    ASSERT_EQ(value, *stored, "Expected inserted value to match the stored value");
}

/**
 * @brief Verify hashmap_put overwrites an existing key without duplicating it.
 *
 * Inserting the same key again should replace the stored value and preserve the
 * map size.
 */
static void test_hashmap_put_updates_existing_key(void)
{
    char key[] = "alpha";
    int first_value = 42;
    int second_value = 99;

    int ret = hashmap_put(&map, key, strlen(key), &first_value);
    ASSERT_EQ(0, ret, "Expected first insertion to succeed");

    ret = hashmap_put(&map, key, strlen(key), &second_value);
    ASSERT_EQ(0, ret, "Expected overwrite of an existing key to succeed");
    ASSERT_EQ(1, map.size, "Expected map size to remain stable when overwriting a key");

    int *stored = (int *)hashmap_get(&map, key, strlen(key));
    ASSERT_NOT_NULL(stored, "Expected overwritten value to remain accessible");
    ASSERT_EQ(second_value, *stored, "Expected the latest value to replace the original one");
}

static void test_hashmap_put_zero_length_key(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, 0, &value);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when key size is zero");
}

void run_hashmap_put_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_put_null_map);
    RUN_TEST(test_hashmap_put_zero_length_key);
    RUN_TEST(test_hashmap_put_lazy_allocation);
    RUN_TEST(test_hashmap_put_valid_insert);
    RUN_TEST(test_hashmap_put_updates_existing_key);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
