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

static void test_hashmap_remove_null_map(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected map to be ready for remove tests");

    ret = hashmap_remove(NULL, key, strlen(key));
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when map pointer is NULL");
}

static void test_hashmap_remove_missing_key(void)
{
    char key[] = "missing";
    int ret = hashmap_remove(&map, key, strlen(key));
    ASSERT_EQ(-ENOENT, ret, "Expected -ENOENT when removing a key that does not exist");
}

static void test_hashmap_remove_existing_key(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected insert to succeed before removal");

    ret = hashmap_remove(&map, key, strlen(key));
    ASSERT_EQ(0, ret, "Expected removal of an existing key to succeed");
    ASSERT_EQ(0, map.size, "Expected size to decrement after removing a key");
    ASSERT_NULL(hashmap_get(&map, key, strlen(key)), "Expected removed key to no longer be readable");
}

static void test_hashmap_remove_zero_length_key(void)
{
    char key[] = "alpha";
    int ret = hashmap_remove(&map, key, 0);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when key size is zero");
}

void run_hashmap_remove_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_remove_null_map);
    RUN_TEST(test_hashmap_remove_missing_key);
    RUN_TEST(test_hashmap_remove_existing_key);
    RUN_TEST(test_hashmap_remove_zero_length_key);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
