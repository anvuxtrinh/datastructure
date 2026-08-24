#include "../inc/vec.h"

#define VECTOR_INITIAL_CAPACITY 4

int vec_init(Vec *self, size_t size, size_t esize) {
    if(self == NULL || esize == 0) { return VEC_ERR_NULL; }

    self->data = malloc(VECTOR_INITIAL_CAPACITY * esize);
    if(self->data == NULL) { return VEC_ERR_ALLOC; }
    self->esize = esize;
    self->size = size;
    self->cap = VECTOR_INITIAL_CAPACITY;
    self->free_cb = NULL;
    return VEC_SUCCESS;
}

int vec_push(Vec *self, void *item) {
    if(self == NULL || item == NULL) { return VEC_ERR_NULL; }

    if(self->size >= self->cap) {
        self->cap *= 2;
        void *new_data = realloc(self->data, self->cap * self->esize);
        if(new_data == NULL) { return VEC_ERR_ALLOC; }
        self->data = new_data;
    }

    memcpy((char *)self->data + (self->size * self->esize), item, self->esize);
    self->size++;
    return VEC_SUCCESS;
}

void* vec_at(Vec *self, size_t index) {
    if(self == NULL) { return NULL; }
    if(index >= self->size) { return NULL; }

    return (char *)self->data + (index * self->esize);
}

int vec_remove(Vec *self, size_t index) {
    if(self == NULL) { return VEC_ERR_NULL; }
    if(index >= self->size) { return VEC_ERR_OUT_OF_BOUNDS; }

    memmove((char *)self->data + (index * self->esize),
            (char *)self->data + ((index + 1) * self->esize),
            (self->size - index - 1) * self->esize);
    self->size--;
    return VEC_SUCCESS;
}

int vec_clear(Vec *self) {
    if(self == NULL) { return VEC_ERR_NULL; }

    if(self->free_cb != NULL) {
        for(size_t i = 0; i < self->size; i++) {
            self->free_cb((char *)self->data + (i * self->esize));
        }
    }

    self->size = 0;
    return VEC_SUCCESS;
}

int vec_free(Vec *self) {
    if(self == NULL) { return VEC_ERR_NULL; }

    vec_clear(self);
    if(self->data != NULL) {
        free(self->data);
        self->data = NULL;
    }
    self->size = 0;
    self->cap = 0;
    return VEC_SUCCESS;
}

void vec_set_free_cb(Vec *self, vec_free_cb_t free_cb) {
    if(self == NULL) { return; }
    self->free_cb = free_cb;
}