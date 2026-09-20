#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "framework/framework.h"
#include "../../inc/vec.h"

// = Test Fixtures
static vec_t vec;
static int raw_clone_call_count;
static int raw_clone_fail_after;

struct raw_payload {
    int id;
    char label[16];
};

static void *clone_string_elem(const void *elem)
{
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

static void free_string_elem(void *elem)
{
    free(*(char **)elem);
}

static const data_ops_t string_ops = {
    .clone = clone_string_elem,
    .free = free_string_elem,
};

static void *clone_raw_payload_elem(const void *elem)
{
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

static const data_ops_t raw_payload_ops = {
    .clone = clone_raw_payload_elem,
    .free = NULL,
};

static void setup(void)
{
    int ret = vec_init(&vec, sizeof(int), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for vec_push tests");
    raw_clone_call_count = 0;
    raw_clone_fail_after = 0;
}

static void teardown(void)
{
    vec_deinit(&vec);
}

// = Null Input Tests
/**
 * @brief Verify vec_push rejects a NULL vector pointer and a NULL element pointer.
 */
static void test_vec_push_null_inputs(void)
{
    int item = 42;
    int ret = vec_push(NULL, &item);
    ASSERT_EQ(EINVAL, ret, "Expected vec_push to return EINVAL when vector pointer is NULL");

    ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_push to return EINVAL when element pointer is NULL");
}

// = Error Code Verification Tests
/**
 * @brief Verify vec_push reports EINVAL when called with a NULL element pointer.
 */
static void test_vec_push_null_element_error_code(void)
{
    int ret = vec_push(&vec, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected vec_push to return EINVAL for a NULL element pointer");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify vec_push stores an integer element and increments size.
 */
static void test_vec_push_valid_integer_value(void)
{
    int item = 42;
    int ret = vec_push(&vec, &item);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed for a valid integer value");
    ASSERT_EQ(1, (int)vec.size, "Expected vec_push to increment size after storing one integer value");

    int *retrieved_item = (int *)vec_at(&vec, 0);
    ASSERT_NOT_NULL(retrieved_item, "Expected vec_at to return the stored integer value after vec_push");
    ASSERT_EQ(item, *retrieved_item, "Expected the stored integer value to match the pushed input");
}

/**
 * @brief Verify vec_push stores a plain struct value without custom operations.
 */
static void test_vec_push_plain_struct_value(void)
{
    struct Point {
        int x;
        int y;
    };

    struct Point p = { .x = 10, .y = 20 };

    vec_t vec_struct;
    int ret = vec_init(&vec_struct, sizeof(struct Point), NULL);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for plain struct storage");

    ret = vec_push(&vec_struct, &p);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed for a plain struct value");
    ASSERT_EQ(1, (int)vec_struct.size, "Expected vec_push to increment size after storing one struct value");

    struct Point *retrieved_point = (struct Point *)vec_at(&vec_struct, 0);
    ASSERT_NOT_NULL(retrieved_point, "Expected vec_at to return the stored struct value");
    ASSERT_EQ(p.x, retrieved_point->x, "Expected stored struct x to match the pushed value");
    ASSERT_EQ(p.y, retrieved_point->y, "Expected stored struct y to match the pushed value");

    vec_deinit(&vec_struct);
}

/**
 * @brief Verify vec_push deep-copies pointer-backed string elements when clone_cb is provided.
 */
static void test_vec_push_deep_copy_pointer_element(void)
{
    vec_t string_vec;
    char source[] = "hello";
    char *input = source;
    int ret = vec_init(&string_vec, sizeof(char *), &string_ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for string ownership operations");

    ret = vec_push(&string_vec, &input);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed for a cloned string pointer element");

    source[0] = 'y';

    char **stored = (char **)vec_at(&string_vec, 0);
    ASSERT_NOT_NULL(stored, "Expected vec_at to return the stored cloned string pointer");
    ASSERT_TRUE(*stored != input, "Expected clone_cb to allocate independent string storage");
    ASSERT_STR_EQ("hello", *stored, "Expected stored string content to remain unchanged after source mutation");

    vec_deinit(&string_vec);
}

/**
 * @brief Verify vec_push frees the temporary clone buffer after copying a raw struct payload.
 */
static void test_vec_push_clone_buffer_for_raw_struct(void)
{
    vec_t raw_vec;
    struct raw_payload item = { .id = 7, .label = "payload" };
    int ret = vec_init(&raw_vec, sizeof(struct raw_payload), &raw_payload_ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for raw payload ownership operations");

    ret = vec_push(&raw_vec, &item);
    ASSERT_EQ(0, ret, "Expected vec_push to clone and store raw payload data");
    ASSERT_EQ(1, raw_clone_call_count, "Expected clone_cb to be called once for the raw payload");
    ASSERT_EQ(1, (int)raw_vec.size, "Expected vector size to increase after storing one raw payload");

    struct raw_payload *stored = (struct raw_payload *)vec_at(&raw_vec, 0);
    ASSERT_NOT_NULL(stored, "Expected stored raw payload to be accessible");
    ASSERT_EQ(item.id, stored->id, "Expected stored raw payload id to match source");
    ASSERT_STR_EQ(item.label, stored->label, "Expected stored raw payload label to match source");

    vec_deinit(&raw_vec);
}

// = Boundary Condition Tests
/**
 * @brief Verify vec_push preserves existing elements when clone_cb fails during a growth attempt.
 */
static void test_vec_push_clone_failure_preserves_existing_elements(void)
{
    vec_t raw_vec;
    int ret = vec_init(&raw_vec, sizeof(struct raw_payload), &raw_payload_ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed before clone failure simulation");

    for(int i = 0; i < 4; i++) {
        struct raw_payload item = { .id = i, .label = "seed" };
        ret = vec_push(&raw_vec, &item);
        ASSERT_EQ(0, ret, "Expected initial raw payload pushes to succeed");
    }

    ASSERT_EQ(4, (int)raw_vec.size, "Expected vector to contain four raw payload elements before clone failure");
    ASSERT_EQ(4, (int)raw_vec.cap, "Expected vector capacity to be full before the failing growth attempt");

    raw_clone_fail_after = raw_clone_call_count + 1;

    struct raw_payload failing_item = { .id = 99, .label = "fail" };
    ret = vec_push(&raw_vec, &failing_item);
    ASSERT_EQ(ENOMEM, ret, "Expected vec_push to propagate ENOMEM when clone_cb fails");
    ASSERT_EQ(4, (int)raw_vec.size, "Expected vector size to remain unchanged after a failed push");

    for(int i = 0; i < 4; i++) {
        struct raw_payload *stored = (struct raw_payload *)vec_at(&raw_vec, (size_t)i);
        ASSERT_NOT_NULL(stored, "Expected previously stored raw payload to remain accessible after failure");
        ASSERT_EQ(i, stored->id, "Expected existing raw payload ids to remain unchanged after failure");
        ASSERT_STR_EQ("seed", stored->label, "Expected existing raw payload labels to remain unchanged after failure");
    }

    vec_deinit(&raw_vec);
}

/**
 * @brief Verify vec_push preserves insertion order across multiple integer values.
 */
static void test_vec_push_multiple_values(void)
{
    int items[] = {1, 2, 3, 4, 5};
    size_t num_items = sizeof(items) / sizeof(items[0]);

    for(size_t i = 0; i < num_items; i++) {
        int ret = vec_push(&vec, &items[i]);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed while storing multiple integer values");
        ASSERT_EQ((int)(i + 1), (int)vec.size, "Expected vec_push to increment size after each insertion");
    }

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected vec_at to return each stored integer value");
        ASSERT_EQ(items[i], *retrieved_item, "Expected insertion order to be preserved for multiple pushes");
    }
}

// = State Transition Tests
/**
 * @brief Verify vec_push grows capacity and preserves values across reallocation.
 */
static void test_vec_push_resize_grows_capacity(void)
{
    size_t initial_capacity = vec.cap;
    size_t num_items = 6;

    for(size_t i = 0; i < num_items; i++) {
        int item = (int)i;
        int ret = vec_push(&vec, &item);
        ASSERT_EQ(0, ret, "Expected vec_push to succeed while growing vector capacity");
        ASSERT_EQ((int)(i + 1), (int)vec.size, "Expected size to increase after each push during growth");
    }

    ASSERT_TRUE(vec.cap > initial_capacity, "Expected capacity to grow after exceeding the initial allocation");

    for(size_t i = 0; i < num_items; i++) {
        int *retrieved_item = (int *)vec_at(&vec, i);
        ASSERT_NOT_NULL(retrieved_item, "Expected stored elements to remain readable after capacity growth");
        ASSERT_EQ((int)i, *retrieved_item, "Expected value preservation after vector capacity growth");
    }
}

void run_vec_push_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_vec_push_null_inputs);
    RUN_TEST(test_vec_push_null_element_error_code);
    RUN_TEST(test_vec_push_valid_integer_value);
    RUN_TEST(test_vec_push_plain_struct_value);
    RUN_TEST(test_vec_push_deep_copy_pointer_element);
    RUN_TEST(test_vec_push_clone_buffer_for_raw_struct);
    RUN_TEST(test_vec_push_clone_failure_preserves_existing_elements);
    RUN_TEST(test_vec_push_multiple_values);
    RUN_TEST(test_vec_push_resize_grows_capacity);

    SET_SETUP(NULL);
    SET_TEARDOWN(NULL);
}