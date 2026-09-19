#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;
static int free_cb_calls;
static int freed_heap_strings;

static void count_free_cb(void *elem)
{
    (void)elem;
    free_cb_calls++;
}

static char *make_heap_string(const char *text)
{
    size_t len = strlen(text) + 1;
    char *copy = malloc(len);

    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, text, len);
    return copy;
}

static void free_heap_string(void *elem)
{
    free(*(char **)elem);
    freed_heap_strings++;
}

static void setup(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for vec_clear tests");
    free_cb_calls = 0;
    freed_heap_strings = 0;
}

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_clear rejects a NULL vector pointer.
 */
static void test_vec_clear_null_vec(void)
{
    int ret = vec_clear(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_clear to return EINVAL for a NULL vector pointer");
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_clear succeeds for an already-empty vector.
 */
static void test_vec_clear_empty_vector(void)
{
    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed when the vector is already empty");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_clear removes stored elements and resets size for plain values.
 */
static void test_vec_clear_with_stored_values(void)
{
    int value = 42;
    int ret = vec_push(&vec, &value);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed before clearing stored values");
    ASSERT_EQ(1, (int)vec.size, "Expected vector size to become one before vec_clear");

    ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed for a vector containing values");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_clear to reset vector size to zero");
}

// = State Transition Tests
/**
 * @brief Verify the vector remains reusable after a clear operation.
 */
static void test_vec_clear_then_push_again(void)
{
    int value = 42;
    int ret = vec_push(&vec, &value);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed before clear-and-reuse scenario");
    ASSERT_EQ(1, (int)vec.size, "Expected vector size to become one before vec_clear");

    ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed before reusing the vector");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_clear to reset size before reusing the vector");

    ret = vec_push(&vec, &value);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed after vec_clear");
    ASSERT_EQ(1, (int)vec.size, "Expected vector size to become one after pushing again");
}

/**
 * @brief Verify vec_clear invokes free_cb once for every stored element.
 */
static void test_vec_clear_calls_free_cb_for_each_element(void)
{
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = count_free_cb,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(int), &ops);
    ASSERT_EQ(0, init_ret, "Expected vec_init to succeed with a counting free_cb");

    int a = 1;
    int b = 2;
    int c = 3;
    ASSERT_EQ(0, vec_push(&vec, &a), "Expected first vec_push to succeed before free_cb counting");
    ASSERT_EQ(0, vec_push(&vec, &b), "Expected second vec_push to succeed before free_cb counting");
    ASSERT_EQ(0, vec_push(&vec, &c), "Expected third vec_push to succeed before free_cb counting");

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed with a counting free_cb");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_clear to reset size after invoking free_cb");
    ASSERT_EQ(3, free_cb_calls, "Expected free_cb to be invoked once for each stored element");
}

/**
 * @brief Verify vec_deinit invokes free_cb through vec_clear before releasing the backing store.
 */
static void test_vec_deinit_calls_free_cb_via_clear(void)
{
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = count_free_cb,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(int), &ops);
    ASSERT_EQ(0, init_ret, "Expected vec_init to succeed with a counting free_cb before vec_deinit");

    int a = 1;
    int b = 2;
    ASSERT_EQ(0, vec_push(&vec, &a), "Expected first vec_push to succeed before vec_deinit free_cb validation");
    ASSERT_EQ(0, vec_push(&vec, &b), "Expected second vec_push to succeed before vec_deinit free_cb validation");

    int ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed and trigger vec_clear for owned elements");
    ASSERT_EQ(2, free_cb_calls, "Expected free_cb to be invoked for every stored element during vec_deinit");
    ASSERT_NULL(vec.data, "Expected vec_deinit to reset data to NULL after releasing storage");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_deinit to reset size to zero");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_deinit to reset capacity to zero");
}

/**
 * @brief Verify vec_clear frees ten heap-backed string elements through free_cb.
 */
static void test_vec_clear_frees_ten_heap_strings(void)
{
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = free_heap_string,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(char *), &ops);
    ASSERT_EQ(0, init_ret, "Expected vec_init to succeed for heap-backed string elements");

    for(int i = 0; i < 10; i++) {
        char buffer[16];
        char *value = NULL;
        int written = snprintf(buffer, sizeof(buffer), "item-%d", i);

        ASSERT_TRUE(written > 0, "Expected snprintf to create a valid heap-string label");
        value = make_heap_string(buffer);
        ASSERT_NOT_NULL(value, "Expected heap string allocation to succeed while preparing vec_clear leak coverage");

        int ret = vec_push(&vec, &value);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed for each heap-backed string element");
    }

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed for heap string elements");
    ASSERT_EQ(10, freed_heap_strings, "Expected free_cb to run exactly once for each of the ten heap-backed strings");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_clear to leave the vector empty after freeing heap-backed strings");
}

/**
 * @brief Verify vec_deinit frees owned heap-backed strings without requiring a prior clear.
 */
static void test_vec_deinit_frees_heap_strings_without_prior_clear(void)
{
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = free_heap_string,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(char *), &ops);
    ASSERT_EQ(0, init_ret, "Expected vec_init to succeed for deinit leak coverage with heap-backed strings");

    for(int i = 0; i < 4; i++) {
        char buffer[16];
        char *value = NULL;
        int written = snprintf(buffer, sizeof(buffer), "data-%d", i);

        ASSERT_TRUE(written > 0, "Expected snprintf to create a valid heap-string label");
        value = make_heap_string(buffer);
        ASSERT_NOT_NULL(value, "Expected heap string allocation to succeed while preparing vec_deinit leak coverage");

        int ret = vec_push(&vec, &value);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed for each heap-backed string before vec_deinit");
    }

    int ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed without explicit clear");
    ASSERT_EQ(4, freed_heap_strings, "Expected free_cb to run for each heap-backed string during vec_deinit");
    ASSERT_NULL(vec.data, "Expected vec.data to be NULL after vec_deinit");
    ASSERT_EQ(0, (int)vec.size, "Expected vec_deinit to reset size after freeing heap-backed strings");
    ASSERT_EQ(0, (int)vec.cap, "Expected vec_deinit to reset capacity after freeing heap-backed strings");
}

// = Error Code Verification Tests
void run_vec_clear_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_clear_null_vec);
    RUN_TEST(test_vec_clear_empty_vector);
    RUN_TEST(test_vec_clear_with_stored_values);
    RUN_TEST(test_vec_clear_then_push_again);
    RUN_TEST(test_vec_clear_calls_free_cb_for_each_element);
    RUN_TEST(test_vec_deinit_calls_free_cb_via_clear);
    RUN_TEST(test_vec_clear_frees_ten_heap_strings);
    RUN_TEST(test_vec_deinit_frees_heap_strings_without_prior_clear);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}