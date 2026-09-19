#pragma once

#include <stdio.h>
#include <stddef.h>

typedef void (*vec_copy_cb_t)(void *dest, const void *src);
typedef void* (*vec_clone_cb_t)(const void *elem);
typedef void (*vec_free_cb_t)(void *elem);
typedef int (*vec_cmp_cb_t)(const void *a, const void *b);

typedef struct vec_ops{
    vec_clone_cb_t clone_cb;
    vec_free_cb_t free_cb;
}vec_ops_t;

typedef struct vec {
    void *data;
    size_t esize;
    size_t size;
    size_t cap;
    const vec_ops_t *ops;
} vec_t;

int vec_init(vec_t *self, size_t esize, const vec_ops_t *ops);
int vec_deinit(vec_t *self);
int vec_push(vec_t *self, const void *elem);
const void* vec_at(const vec_t *self, size_t index);
int vec_remove(vec_t *self, size_t index);
int vec_clear(vec_t *self);
int vec_shrink_to_fit(vec_t *self);