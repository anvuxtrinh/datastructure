#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

static vec_t vec;
static int raw_clone_call_count;
static int raw_clone_fail_after;

struct raw_payload {
    int id;
    char label[16];
};

static void *clone_string_elem(const void *elem) {
    const char *src = *(const char * const *)elem;
    char *copy = malloc(strlen(src) + 1);
    char **boxed = NULL;

    if(copy == NULL) {
        return NULL;
    }

    strcpy(copy, src);

    boxed = malloc(sizeof(*boxed));
    if(boxed == NULL) {
        free(copy);
        return NULL;
    }

    *boxed = copy;
    return boxed;
}

static void free_string_elem(void *elem) {
    free(*(char **)elem);
}

static const vec_ops_t string_ops = {
    .clone_cb = clone_string_elem,
    .free_cb = free_string_elem,
};

static void *clone_raw_payload_elem(const void *elem) {
    const struct raw_payload *src = elem;
    struct raw_payload *copy = NULL;

    raw_clone_call_count++;
    if(raw_clone_fail_after != 0 && raw_clone_call_count >= raw_clone_fail_after) {
        return NULL;
    }

    copy = malloc(sizeof(*copy));
    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, src, sizeof(*copy));
    return copy;
}

static const vec_ops_t raw_payload_ops = {
    .clone_cb = clone_raw_payload_elem,
    .free_cb = NULL,
};

static void setup() {
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    raw_clone_call_count = 0;
    raw_clone_fail_after = 0;
}

static void teardown() {
    vec_deinit(&vec);
}

static void test_vec_push_null() {
    int item = 42;
    int ret = vec_push(NULL, &item);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");

    ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

static void test_vec_push_null_data() {
    int ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected condition to hold");
}

static void test_vec_push_valid() {
    int item = 42;
    int ret = vec_push(&vec, &item);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec.size, "Expected condition to hold");

    int *retrieved_item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
    ASSERT_EQ(item, *retrieved_item, "Expected condition to hold");
}

static void test_vec_push_struct() {
    struct Point {
        int x;
        int y;
    };

    struct Point p = { .x = 10, .y = 20 };

    vec_t vec_struct;
    int ret = vec_init(&vec_struct, sizeof(struct Point), NULL);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_push(&vec_struct, &p);
    ASSERT_EQ(0, ret, "Expected condition to hold");
    ASSERT_EQ(1, vec_struct.size, "Expected condition to hold");

    struct Point *retrieved_point = (struct Point *)vec_at(&vec_struct, 0);
    ASSERT_NOT_NULL(retrieved_point, "Expected pointer to be non-NULL");
    ASSERT_EQ(p.x, retrieved_point->x, "Expected condition to hold");
    ASSERT_EQ(p.y, retrieved_point->y, "Expected condition to hold");

    vec_deinit(&vec_struct);
}

static void test_vec_push_deep_copy_pointer_element() {
    vec_t string_vec;
    char source[] = "hello";
    char *input = source;
    int ret = vec_init(&string_vec, sizeof(char *), &string_ops);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_push(&string_vec, &input);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    source[0] = 'y';

    char **stored = (char **)vec_at(&string_vec, 0);
    ASSERT_NOT_NULL(stored, "Expected pointer to be non-NULL");
    ASSERT_TRUE(*stored != input, "Expected condition to be true");
    ASSERT_STR_EQ("hello", *stored, "Expected strings to be equal");

    vec_deinit(&string_vec);
}

static void test_vec_push_clone_buffer_for_raw_struct(void) {
    vec_t raw_vec;
    struct raw_payload item = { .id = 7, .label = "payload" };
    int ret = vec_init(&raw_vec, sizeof(struct raw_payload), &raw_payload_ops);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    ret = vec_push(&raw_vec, &item);
    ASSERT_EQ(0, ret, "Expected vec_push to clone and store raw payload data");
    ASSERT_EQ(1, raw_clone_call_count, "Expected clone_cb to be called once for the raw payload");
    ASSERT_EQ(1, raw_vec.size, "Expected vector size to increase after raw payload push");

    struct raw_payload *stored = (struct raw_payload *)vec_at(&raw_vec, 0);
    ASSERT_NOT_NULL(stored, "Expected stored raw payload to be accessible");
    ASSERT_EQ(item.id, stored->id, "Expected stored raw payload id to match source");
    ASSERT_STR_EQ(item.label, stored->label, "Expected stored raw payload label to match source");

    vec_deinit(&raw_vec);
}

static void test_vec_push_clone_failure_preserves_existing_elements(void) {
    vec_t raw_vec;
    int ret = vec_init(&raw_vec, sizeof(struct raw_payload), &raw_payload_ops);
    ASSERT_EQ(0, ret, "Expected condition to hold");

    for(int i = 0; i < 4; i++) {
        struct raw_payload item = { .id = i, .label = "seed" };
        ret = vec_push(&raw_vec, &item);
        ASSERT_EQ(0, ret, "Expected initial raw payload pushes to succeed");
    }

    ASSERT_EQ(4, raw_vec.size, "Expected vector to contain four raw payload elements before failure");
    ASSERT_EQ(4, raw_vec.cap, "Expected vector capacity to be full before growth attempt");

    raw_clone_fail_after = raw_clone_call_count + 1;

    struct raw_payload failing_item = { .id = 99, .label = "fail" };
    ret = vec_push(&raw_vec, &failing_item);
    ASSERT_EQ(ENOMEM, ret, "Expected vec_push to propagate ENOMEM when clone_cb fails");
    ASSERT_EQ(4, raw_vec.size, "Expected vector size to remain unchanged after failed push");

    for(int i = 0; i < 4; i++) {
        struct raw_payload *stored = (struct raw_payload *)vec_at(&raw_vec, (size_t)i);
        ASSERT_NOT_NULL(stored, "Expected previously stored raw payload to remain accessible after failure");
        ASSERT_EQ(i, stored->id, "Expected existing raw payload ids to remain unchanged after failure");
        ASSERT_STR_EQ("seed", stored->label, "Expected existing raw payload labels to remain unchanged after failure");
    }

    vec_deinit(&raw_vec);
}

static void test_vec_push_multiple() {
    int items[] = {1, 2, 3, 4, 5};
    size_t num_items = sizeof(items) / sizeof(items[0]);

    for(size_t i = 0; i < num_items; i++) {
        int ret = vec_push(&vec, &items[i]);
        ASSERT_EQ(0, ret, "Expected condition to hold");
        ASSERT_EQ(i + 1, vec.size, "Expected condition to hold");
    }

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
        ASSERT_EQ(items[i], *retrieved_item, "Expected condition to hold");
    }
}

static void test_vec_push_resize() {
    size_t initial_capacity = vec.cap;
    size_t num_items = 6;

    for(size_t i = 0; i < num_items; i++) {
        int item = (int)i;
        int ret = vec_push(&vec, &item);
        ASSERT_EQ(0, ret, "Expected condition to hold");
        ASSERT_EQ(i + 1, vec.size, "Expected condition to hold");
    }

    ASSERT_TRUE(vec.cap > initial_capacity, "Expected condition to be true");

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected pointer to be non-NULL");
        ASSERT_EQ((int)i, *retrieved_item, "Expected condition to hold");
    }
}

void run_vec_push_tests() {
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_push_null);
    RUN_TEST(test_vec_push_null_data);
    RUN_TEST(test_vec_push_valid);
    RUN_TEST(test_vec_push_struct);
    RUN_TEST(test_vec_push_deep_copy_pointer_element);
    RUN_TEST(test_vec_push_clone_buffer_for_raw_struct);
    RUN_TEST(test_vec_push_clone_failure_preserves_existing_elements);
    RUN_TEST(test_vec_push_multiple);
    RUN_TEST(test_vec_push_resize);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}