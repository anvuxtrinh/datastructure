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
    ASSERT_EQ(0, ret, "Expected trie_init to initialize the cleanup test root");
}

static void teardown(void)
{
    free(root);
    root = NULL;
}

/* = Null Input Tests */
/**
 * @brief Verify trie_free_child safely handles a NULL node pointer.
 *
 * Passing NULL to the recursive cleanup helper should be a no-op and must not cause a crash
 * or access violation.
 */
void test_trie_free_child_null(void)
{
    trie_free_child(NULL);
    ASSERT_TRUE(true, "Expected trie_free_child to return without error when the node is NULL");
}

/* = Boundary Condition Tests */
/**
 * @brief Verify trie_free_child handles leaf and populated subtrees without crashing.
 *
 * A node with no children and a node with multiple descendants should both be accepted by the
 * recursive cleanup routine.
 */
void test_trie_free_child_handles_tree(void)
{
    int ret = trie_insert(root, "cat", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'cat' to succeed before cleanup");

    ret = trie_insert(root, "car", 3);
    ASSERT_EQ(0, ret, "Expected insertion of 'car' to succeed before cleanup");

    trie_free_child(root);
    ASSERT_TRUE(true, "Expected trie_free_child to complete cleanup on a populated trie subtree");
}

/* = Valid Behavior Tests (Happy Path) */
/**
 * @brief Verify trie_free_child can clean up a freshly initialized node.
 *
 * A newly initialized trie root should be safe to pass through the recursive cleanup helper,
 * because all child pointers start as NULL.
 */
void test_trie_free_child_empty_tree(void)
{
    trie_free_child(root);
    ASSERT_TRUE(true, "Expected trie_free_child to handle an empty root without crashing");
}

void run_trie_free_child_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_trie_free_child_null);
    RUN_TEST(test_trie_free_child_handles_tree);
    RUN_TEST(test_trie_free_child_empty_tree);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}
