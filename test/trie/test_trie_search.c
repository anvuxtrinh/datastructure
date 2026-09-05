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
    ASSERT_EQ(0, ret, "Expected trie_init to initialize the search test root");
}

static void teardown(void)
{
    free(root);
    root = NULL;
}

/* = Null Input Tests */
/**
 * @brief Verify trie_search rejects NULL trie roots and NULL word buffers.
 *
 * Search should never dereference invalid pointers; it should return false whenever the
 * trie or the queried word is missing or invalid.
 */
void test_trie_search_null_inputs(void)
{
    const char *word = "cat";

    ASSERT_FALSE(trie_search(NULL, word, 3), "Expected trie_search to return false for a NULL trie root");
    ASSERT_FALSE(trie_search(root, NULL, 3), "Expected trie_search to return false for a NULL word pointer");
}

/* = Boundary Condition Tests */
/**
 * @brief Verify trie_search returns false for unknown, invalid, and prefix-only lookups.
 *
 * Missing entries and characters outside the lowercase alphabet are invalid query inputs and
 * must not produce a true result.
 */
void test_trie_search_invalid_queries(void)
{
    const char *word = "cat";

    int ret = trie_insert(root, word, 3);
    ASSERT_EQ(0, ret, "Expected valid insertion before running lookup checks");

    ASSERT_FALSE(trie_search(root, "dog", 3), "Expected trie_search to return false for a missing word");
    ASSERT_FALSE(trie_search(root, "ca", 2), "Expected trie_search to return false for a prefix-only lookup");
    ASSERT_FALSE(trie_search(root, "Cat", 3), "Expected trie_search to return false for uppercase letters");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_search returns true for an inserted word and false for non-existent data.
 *
 * Once a word is inserted, searching for that exact key should succeed and the trie should
 * preserve the terminal state of the final node.
 */
void test_trie_search_valid_word(void)
{
    const char *word = "cat";

    int ret = trie_insert(root, word, 3);
    ASSERT_EQ(0, ret, "Expected trie_insert to succeed before lookup");

    ASSERT_TRUE(trie_search(root, word, 3), "Expected trie_search to find a word that was inserted successfully");
    ASSERT_FALSE(trie_search(root, "car", 3), "Expected trie_search to reject a different word with a shared prefix");
}

/* = State Transition Tests */
/**
 * @brief Verify trie_search behaves correctly after inserting multiple words and empty-string data.
 *
 * Searching should reflect the current trie state after updates, including empty-string
 * terminal markers and the insertion of additional sibling words.
 */
void test_trie_search_after_updates(void)
{
    int ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected the first insertion to succeed");

    ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected the second insertion to succeed");

    ret = trie_insert(root, "", 0);
    ASSERT_EQ(0, ret, "Expected insertion of the empty string to succeed");

    ASSERT_TRUE(trie_search(root, "car", 3), "Expected 'car' to remain searchable after shared-prefix insertion");
    ASSERT_TRUE(trie_search(root, "cat", 3), "Expected 'cat' to remain searchable after a sibling insertion");
    ASSERT_TRUE(trie_search(root, "", 0), "Expected the empty string sentinel to be searchable after insertion");
}

void run_trie_search_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_search_null_inputs);
    RUN_TEST(test_trie_search_invalid_queries);
    RUN_TEST(test_trie_search_valid_word);
    RUN_TEST(test_trie_search_after_updates);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
