#pragma once

#include <stdio.h>

enum {
    VEC_SUCCESS = 0,
    VEC_ERR_ALLOC = 1,
    VEC_ERR_NULL = 2,
    VEC_ERR_OUT_OF_BOUNDS = 3
};

typedef void (*vec_free_cb_t)(void *elem);

typedef struct vector {
    void *data;
    size_t esize;
    size_t size;
    size_t cap;
    vec_free_cb_t free_cb;
} Vec;

int vec_init(Vec *self, size_t size, size_t esize);
int vec_push(Vec *self, void *elem);
void* vec_at(Vec *self, size_t index);
int vec_remove(Vec *self, size_t index);
int vec_clear(Vec *self);
int vec_free(Vec *self);
void vec_set_free_cb(Vec *self, vec_free_cb_t free_cb);