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

static void test_hashmap_contains_null_map(void)
{
    char key[] = "alpha";
    ASSERT_FALSE(hashmap_contains(NULL, key, strlen(key)), "Expected NULL map to report no membership");
}

static void test_hashmap_contains_missing_key(void)
{
    char key[] = "missing";
    ASSERT_FALSE(hashmap_contains(&map, key, strlen(key)), "Expected missing key to not be present");
}

static void test_hashmap_contains_existing_key(void)
{
    char key[] = "alpha";
    int value = 42;

    int ret = hashmap_put(&map, key, strlen(key), &value);
    ASSERT_EQ(0, ret, "Expected insertion to succeed before membership check");
    ASSERT_TRUE(hashmap_contains(&map, key, strlen(key)), "Expected inserted key to be reported as present");
}

static void test_hashmap_contains_null_value(void)
{
    char key[] = "alpha";
    int *value = NULL;

    int ret = hashmap_put(&map, key, strlen(key), value);
    ASSERT_EQ(0, ret, "Expected insertion to accept a NULL stored value");
    ASSERT_TRUE(hashmap_contains(&map, key, strlen(key)), "Expected contains to detect a present key even when its value is NULL");
}

void run_hashmap_contains_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_hashmap_contains_null_map);
    RUN_TEST(test_hashmap_contains_missing_key);
    RUN_TEST(test_hashmap_contains_existing_key);
    RUN_TEST(test_hashmap_contains_null_value);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
