#include <stdlib.h>
#include <string.h>

#include "framework/framework.h"
#include "../../inc/hashmap.h"

unsigned int hashmap_hash(any_t key, size_t ksize);

// = Test Fixtures

// = Boundary Condition Tests
/**
 * @brief Verify hash generation for a zero-length key is stable and deterministic.
 *
 * A zero-length key should still produce a consistent hash value instead of
 * reading outside the buffer.
 */
static void test_hashmap_hash_zero_length(void)
{
    char key[] = "alpha";
    unsigned int hash = hashmap_hash(key, 0);
    ASSERT_EQ(5381u, hash, "Expected zero-length keys to map to the base hash value");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify identical keys always produce identical hashes.
 *
 * Determinism is the core requirement of hashing: the same bytes and size must
 * generate the same bucket index every time.
 */
static void test_hashmap_hash_stable_for_same_key(void)
{
    char key[] = "alpha";
    unsigned int first_hash = hashmap_hash(key, strlen(key));
    unsigned int second_hash = hashmap_hash(key, strlen(key));
    ASSERT_EQ(first_hash, second_hash, "Expected the same key to produce the same hash");
}

/**
 * @brief Verify different keys generate different hash outputs.
 *
 * Distinct keys should not collapse to the same hash value, otherwise bucket
 * distribution becomes poor and collisions increase.
 */
static void test_hashmap_hash_distinguishes_keys(void)
{
    char first_key[] = "alpha";
    char second_key[] = "beta";
    unsigned int first_hash = hashmap_hash(first_key, strlen(first_key));
    unsigned int second_hash = hashmap_hash(second_key, strlen(second_key));
    ASSERT_TRUE(first_hash != second_hash, "Expected different keys to produce different hashes");
}

// = State Transition Tests

// = Error Code Verification Tests

void run_hashmap_hash_tests(void)
{
    RUN_TEST(test_hashmap_hash_zero_length);
    RUN_TEST(test_hashmap_hash_stable_for_same_key);
    RUN_TEST(test_hashmap_hash_distinguishes_keys);
}
