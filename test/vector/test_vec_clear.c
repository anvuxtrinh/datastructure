#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;
static int free_cb_calls;
static int freed_heap_strings;

static void count_free_cb(void *elem) {
    (void)elem;
    free_cb_calls++;
}

static char *make_heap_string(const char *text) {
    size_t len = strlen(text) + 1;
    char *copy = malloc(len);

    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, text, len);
    return copy;
}

static void free_heap_string(void *elem) {
    free(*(char **)elem);
    freed_heap_strings++;
}

static void setup() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    free_cb_calls = 0;
    freed_heap_strings = 0;
}

static void teardown() {
    vec_deinit(&vec);
}

void test_vec_clear_null() {
    int ret = vec_clear(NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

void test_vec_clear_empty() {
    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
}

void test_vec_clear_with_data() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
}

void test_vec_clear_then_push_again() {
    int value = 42;
    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");

    vec_push(&vec, &value);
    ASSERT_EQ(1, vec.size, "Expected condition to hold");
}

void test_vec_clear_calls_free_cb_for_each_element() {
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = count_free_cb,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(int), &ops);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    int a = 1;
    int b = 2;
    int c = 3;
    vec_push(&vec, &a);
    vec_push(&vec, &b);
    vec_push(&vec, &c);

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(3, free_cb_calls, "Expected condition to hold");
}

void test_vec_free_calls_free_cb_via_clear() {
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = count_free_cb,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(int), &ops);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    int a = 1;
    int b = 2;
    vec_push(&vec, &a);
    vec_push(&vec, &b);

    int ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(2, free_cb_calls, "Expected condition to hold");
    ASSERT_NULL(vec.data, "Expected pointer to be NULL");
    ASSERT_EQ(0, vec.size, "Expected condition to hold");
    ASSERT_EQ(0, vec.cap, "Expected condition to hold");
}

void test_vec_clear_frees_ten_heap_strings(void) {
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = free_heap_string,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(char *), &ops);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    for(int i = 0; i < 10; i++) {
        char buffer[16];
        char *value = NULL;
        int written = snprintf(buffer, sizeof(buffer), "item-%d", i);

        ASSERT_TRUE(written > 0, "Expected snprintf to create heap-string input");
        value = make_heap_string(buffer);
        ASSERT_NOT_NULL(value, "Expected heap string allocation to succeed");

        int ret = vec_push(&vec, &value);
        ASSERT_EQ(0, ret, "Expected pushing heap string pointers to succeed");
    }

    int ret = vec_clear(&vec);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed for heap string elements");
    ASSERT_EQ(10, freed_heap_strings, "Expected free_cb to run once for each heap string");
    ASSERT_EQ(0, vec.size, "Expected vector size to be zero after vec_clear");
}

void test_vec_deinit_frees_heap_strings_without_prior_clear(void) {
    static const vec_ops_t ops = {
        .clone_cb = NULL,
        .free_cb = free_heap_string,
    };

    vec_deinit(&vec);
    int init_ret = vec_init(&vec, sizeof(char *), &ops);
    ASSERT_EQ(0, init_ret, "Expected condition to hold");

    for(int i = 0; i < 4; i++) {
        char buffer[16];
        char *value = NULL;
        int written = snprintf(buffer, sizeof(buffer), "data-%d", i);

        ASSERT_TRUE(written > 0, "Expected snprintf to create heap-string input");
        value = make_heap_string(buffer);
        ASSERT_NOT_NULL(value, "Expected heap string allocation to succeed");

        int ret = vec_push(&vec, &value);
        ASSERT_EQ(0, ret, "Expected pushing heap string pointers to succeed");
    }

    int ret = vec_deinit(&vec);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed without explicit clear");
    ASSERT_EQ(4, freed_heap_strings, "Expected free_cb to run for each heap string during deinit");
    ASSERT_NULL(vec.data, "Expected vec.data to be NULL after vec_deinit");
    ASSERT_EQ(0, vec.size, "Expected vector size to be reset after vec_deinit");
    ASSERT_EQ(0, vec.cap, "Expected vector capacity to be reset after vec_deinit");
}

void run_vec_clear_tests(void) {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_clear_null);
    RUN_TEST(test_vec_clear_empty);
    RUN_TEST(test_vec_clear_with_data);
    RUN_TEST(test_vec_clear_then_push_again);
    RUN_TEST(test_vec_clear_calls_free_cb_for_each_element);
    RUN_TEST(test_vec_free_calls_free_cb_via_clear);
    RUN_TEST(test_vec_clear_frees_ten_heap_strings);
    RUN_TEST(test_vec_deinit_frees_heap_strings_without_prior_clear);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}