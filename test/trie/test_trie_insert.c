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
    ASSERT_EQ(0, ret, "Expected trie_init to initialize the test root");
}

static void teardown(void)
{
    free(root);
    root = NULL;
}

/* = Null Input Tests */
/**
 * @brief Verify trie_insert rejects NULL root and NULL word pointers.
 *
 * Invalid arguments must return EINVAL and must never attempt to dereference a NULL
 * trie or word buffer.
 */
void test_trie_insert_null_inputs(void)
{
    const char *word = "cat";

    int ret = trie_insert(NULL, word, 3);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when the trie root pointer is NULL");

    ret = trie_insert(root, NULL, 3);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when the insertion word pointer is NULL");
}

/* = Boundary Condition Tests */
/**
 * @brief Verify trie_insert rejects invalid characters and empty valid words are handled consistently.
 *
 * Only lowercase alphabetic characters are valid in trie words. Empty strings should not
 * be treated as an error and should mark the root as a terminal node.
 */
void test_trie_insert_invalid_inputs(void)
{
    int ret = trie_insert(root, "Cat", 3);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when a word contains uppercase characters");

    ret = trie_insert(root, "dog-1", 5);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when a word contains a non-lowercase character");

    ret = trie_insert(root, "", 0);
    ASSERT_EQ(0, ret, "Expected an empty string insertion to succeed");
    ASSERT_TRUE(root->is_end_of_word, "Expected the root node to become terminal after inserting an empty word");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_insert stores a valid word under the expected path.
 *
 * After insertion, each character node must be allocated, the terminal flag must be set for
 * the final character, and the path must remain traversable for later searches.
 */
void test_trie_insert_valid_word(void)
{
    const char *word = "cat";

    int ret = trie_insert(root, word, 3);
    ASSERT_EQ(0, ret, "Expected trie_insert to succeed for a valid lowercase word");

    ASSERT_NOT_NULL(root->child['c' - 'a'], "Expected the 'c' child to exist after inserting 'cat'");
    ASSERT_NOT_NULL(root->child['c' - 'a']->child['a' - 'a'], "Expected the 'a' child to exist under 'c'");
    ASSERT_NOT_NULL(root->child['c' - 'a']->child['a' - 'a']->child['t' - 'a'], "Expected the 't' child to exist under 'ca'");
    ASSERT_TRUE(root->child['c' - 'a']->child['a' - 'a']->child['t' - 'a']->is_end_of_word,
                "Expected the final node in 'cat' to be marked as terminal");
}

/* = State Transition Tests */
/**
 * @brief Verify trie_insert supports shared prefixes without clobbering sibling paths.
 *
 * Words such as 'car' and 'cat' share the same prefix but must keep separate terminal nodes
 * while preserving the common branch structure.
 */
void test_trie_insert_shared_prefixes(void)
{
    int ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected the first shared-prefix insertion to succeed");

    ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected the second shared-prefix insertion to succeed");

    ASSERT_NOT_NULL(root->child['c' - 'a'], "Expected the shared 'c' branch to remain present");
    ASSERT_NOT_NULL(root->child['c' - 'a']->child['a' - 'a'], "Expected the shared 'ca' branch to remain present");
    ASSERT_TRUE(root->child['c' - 'a']->child['a' - 'a']->child['r' - 'a']->is_end_of_word,
                "Expected 'car' to remain a terminal word after inserting 'cat'");
    ASSERT_TRUE(root->child['c' - 'a']->child['a' - 'a']->child['t' - 'a']->is_end_of_word,
                "Expected 'cat' to remain a terminal word after inserting 'car'");
}

void run_trie_insert_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_insert_null_inputs);
    RUN_TEST(test_trie_insert_invalid_inputs);
    RUN_TEST(test_trie_insert_valid_word);
    RUN_TEST(test_trie_insert_shared_prefixes);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
