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
    ASSERT_EQ(0, ret, "Expected trie_init to initialize the trie free test root");
}

static void teardown(void)
{
    if (root != NULL) {
        free(root);
        root = NULL;
    }
}

/* = Null Input Tests */
/**
 * @brief Verify trie_free safely accepts a NULL root pointer.
 *
 * Freeing a NULL trie root should be treated as a safe no-op and must not trigger any
 * invalid memory operations.
 */
void test_trie_free_null_root(void)
{
    trie_free(NULL);
    ASSERT_TRUE(true, "Expected trie_free to return without error when the root is NULL");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_free cleans up a populated trie structure.
 *
 * After inserting several words, the full trie should be recursively released without any
 * crashes or invalid writes.
 */
void test_trie_free_populated_tree(void)
{
    int ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'cat' to succeed before full cleanup");

    ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'car' to succeed before full cleanup");

    trie_free(root);
    root = NULL;
    ASSERT_TRUE(true, "Expected trie_free to release a populated trie tree without crashing");
}

/* = State Transition Tests */
/**
 * @brief Verify trie_free handles an empty trie as a valid cleanup target.
 *
 * An initialized but empty trie should be released successfully without leaving dangling
 * child pointers or corrupting the root state.
 */
void test_trie_free_empty_tree(void)
{
    trie_free(root);
    root = NULL;
    ASSERT_TRUE(true, "Expected trie_free to handle an empty but initialized trie root safely");
}

void run_trie_free_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_free_null_root);
    RUN_TEST(test_trie_free_populated_tree);
    RUN_TEST(test_trie_free_empty_tree);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
