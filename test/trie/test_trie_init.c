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
    ASSERT_EQ(0, ret, "Expected trie_init to initialize a valid root node");
}

static void teardown(void)
{
    free(root);
    root = NULL;
}

/* = Null Input Tests */
/**
 * @brief Verify trie_init rejects a NULL root pointer.
 *
 * A NULL trie root should be treated as an invalid argument and return -EINVAL without
 * dereferencing the pointer.
 */
void test_trie_init_null_root(void)
{
    int ret = trie_init(NULL);
    ASSERT_EQ(-EINVAL, ret, "Expected -EINVAL when the trie root pointer is NULL");
}

/* = Boundary Condition Tests */
/**
 * @brief Verify trie_init clears the root node state for a freshly allocated trie.
 *
 * All child pointers must be NULL and the end-of-word flag must be reset to false after
 * initialization so the trie remains empty and ready for insertions.
 */
void test_trie_init_clears_state(void)
{
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        ASSERT_NULL(root->child[i], "Expected every child pointer to be NULL after trie_init");
    }

    ASSERT_FALSE(root->is_end_of_word, "Expected the root node to start as a non-terminal word");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_init successfully initializes a valid root node.
 *
 * The root should be usable immediately after initialization and must not contain any
 * partial or terminal entries before words are inserted.
 */
void test_trie_init_valid_root(void)
{
    int ret = trie_init(root);
    ASSERT_EQ(0, ret, "Expected trie_init to succeed when the root pointer is valid");

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        ASSERT_NULL(root->child[i], "Expected all child slots to remain NULL after a repeated initialization");
    }

    ASSERT_FALSE(root->is_end_of_word, "Expected a freshly initialized trie to be empty");
}

void run_trie_init_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_init_valid_root);
    RUN_TEST(test_trie_init_null_root);
    RUN_TEST(test_trie_init_clears_state);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
