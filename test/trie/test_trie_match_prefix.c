#include <errno.h>
#include <stdlib.h>

#include "framework/framework.h"
#include "../../inc/trie.h"

/* = Test Fixtures */
static trie_node_t *root = NULL;

static void setup(void)
{
    root = malloc(sizeof(*root));
    ASSERT_NOT_NULL(root, "Expected trie node allocation to succeed for setup");

    int ret = trie_init(root);
    ASSERT_EQ(0, ret, "Expected trie_init to initialize the prefix test root");
}

static void teardown(void)
{
    free(root);
    root = NULL;
}

/* = Null Input Tests */
/**
 * @brief Verify trie_match_prefix rejects NULL trie roots and NULL words.
 *
 * Prefix matching should return -1 when the root or the queried word is invalid instead of
 * dereferencing a bad pointer.
 */
void test_trie_match_prefix_null_inputs(void)
{
    ASSERT_EQ(-1, trie_match_prefix(NULL, "cat", 3), "Expected -1 when the trie root pointer is NULL");
    ASSERT_EQ(-1, trie_match_prefix(root, NULL, 3), "Expected -1 when the word pointer is NULL");
}

/* = Boundary Condition Tests */
/**
 * @brief Verify trie_match_prefix rejects invalid characters and returns partial matches.
 *
 * Non-lowercase letters are invalid for the trie index map, and unmatched paths should return
 * the number of characters already matched before the first missing edge.
 */
void test_trie_match_prefix_invalid_and_partial(void)
{
    int ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected valid insertion before prefix checks");

    ASSERT_EQ(-1, trie_match_prefix(root, "CAt", 3), "Expected -1 when the lookup contains uppercase characters");
    ASSERT_EQ(2, trie_match_prefix(root, "ca", 2), "Expected a prefix of length 2 to match the stored word 'cat'");
    ASSERT_EQ(1, trie_match_prefix(root, "cag", 3), "Expected one character to match before the missing 'g' branch");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_match_prefix returns the full matching length for inserted words.
 *
 * When the entire query matches an existing path in the trie, the function should return the
 * number of valid nodes traversed, which equals the query length.
 */
void test_trie_match_prefix_full_match(void)
{
    int ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'car' to succeed");

    ASSERT_EQ(3, trie_match_prefix(root, "car", 3), "Expected full match to return the length of the inserted word");
    ASSERT_EQ(2, trie_match_prefix(root, "ca", 2), "Expected the common prefix 'ca' to match before the final branch");
}

/* = State Transition Tests */
/**
 * @brief Verify trie_match_prefix follows the current trie state after inserting multiple words.
 *
 * Shared prefixes should keep common path lengths stable while independent leaf nodes still
 * produce the correct match counts for each word.
 */
void test_trie_match_prefix_shared_prefixes(void)
{
    int ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'car' to succeed");

    ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'cat' to succeed");

    ASSERT_EQ(3, trie_match_prefix(root, "car", 3), "Expected 'car' to match fully after a sibling insert");
    ASSERT_EQ(3, trie_match_prefix(root, "cat", 3), "Expected 'cat' to match fully after a sibling insert");
    ASSERT_EQ(2, trie_match_prefix(root, "ca", 2), "Expected the shared prefix 'ca' to remain valid after inserting both words");
}

void run_trie_match_prefix_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_match_prefix_null_inputs);
    RUN_TEST(test_trie_match_prefix_invalid_and_partial);
    RUN_TEST(test_trie_match_prefix_full_match);
    RUN_TEST(test_trie_match_prefix_shared_prefixes);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
