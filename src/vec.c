#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "../inc/vec.h"


#define VECTOR_INITIAL_CAPACITY 4
#define VECTOR_GROWTH_FACTOR 2

static inline bool should_grow(vec_t *self) {
    return self->size >= self->cap;
}

static inline void* get_element_ptr(vec_t *self, size_t index) {
    return (char *)self->data + (index * self->esize);
}

int vec_init(vec_t *self, size_t esize, vec_free_cb_t free_cb) {
    if(self == NULL || esize == 0) { return EINVAL; }

    self->data = NULL;
    self->esize = esize;
    self->size = 0;
    self->cap = 0;
    self->free_cb = free_cb;
    return 0;
}

int vec_push(vec_t *self, void *item) {
    if(self == NULL || item == NULL) { return EINVAL; }

    if(self->cap == 0) {
        self->cap = VECTOR_INITIAL_CAPACITY;
        self->data = malloc(self->cap * self->esize);
        if(self->data == NULL) { return ENOMEM; }
    }

    if(should_grow(self)) {
        self->cap *= VECTOR_GROWTH_FACTOR;
        void *new_data = realloc(self->data, self->cap * self->esize);
        if(new_data == NULL) { return ENOMEM; }
        self->data = new_data;
    }

    memcpy(get_element_ptr(self, self->size), item, self->esize);
    self->size++;
    return 0;
}

void* vec_at(vec_t *self, size_t index) {
    if(self == NULL) { return NULL; }
    if(index >= self->size) { return NULL; }

    return get_element_ptr(self, index);
}

int vec_remove(vec_t *self, size_t index) {
    if(self == NULL) { return EINVAL; }
    if(index >= self->size) { return ERANGE; }

    void *elem_ptr = get_element_ptr(self, index);
    if(self->free_cb != NULL) {
        self->free_cb(elem_ptr);
    }

    if(index < self->size - 1) {
        void *next_elem_ptr = get_element_ptr(self, index + 1);
        memmove(elem_ptr, next_elem_ptr, (self->size - index - 1) * self->esize);
    }

    self->size--;
    return 0;
}

int vec_clear(vec_t *self) {
    if(self == NULL) { return EINVAL; }

    if(self->free_cb != NULL) {
        for(size_t i = 0; i < self->size; i++) {
            self->free_cb((char *)self->data + (i * self->esize));
        }
    }

    self->size = 0;
    return 0;
}

int vec_free(vec_t *self) {
    if(self == NULL) { return EINVAL; }

    vec_clear(self);
    if(self->data != NULL) {
        free(self->data);
        self->data = NULL;
    }
    self->size = 0;
    self->cap = 0;
    return 0;
}