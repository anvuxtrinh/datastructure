#include <errno.h>
#include <string.h>

#include "framework/framework.h"
#include "../../inc/hashmap.h"

static void test_hashmap_init_initializes_empty_map(void)
{
    hashmap_t map = {123, 456, (struct hashmap_node **)0x1};

    int ret = hashmap_init(&map);
    ASSERT_EQ(0, ret, "Expected hashmap_init to succeed for a valid map");
    ASSERT_EQ(0, map.size, "Expected size to reset to zero during initialization");
    ASSERT_EQ(0, map.cap, "Expected capacity to reset to zero during initialization");
    ASSERT_NULL(map.buckets, "Expected bucket table to be cleared during initialization");
}

static void test_hashmap_init_null_map(void)
{
    int ret = hashmap_init(NULL);
    ASSERT_EQ(-EINVAL, ret, "Expected hashmap_init to reject a NULL map pointer");
}

void run_hashmap_init_tests(void)
{
    RUN_TEST(test_hashmap_init_initializes_empty_map);
    RUN_TEST(test_hashmap_init_null_map);
}
