#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    CSTR_SUCCESS = 0,
    CSTR_ERR_ALLOC = 1,
    CSTR_ERR_NULL = 2,
    CSTR_ERR_OUT_OF_BOUNDS = 3
};

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} cstr;

void cstr_init(cstr *self);
int cstr_appendn(cstr *self, const char *str, size_t n);
int cstr_pop(cstr *self);
int cstr_copy(cstr *self, const char *str, size_t n);
char cstr_at(cstr *self, size_t index);
int cstr_substr(cstr *self, size_t start, size_t end, cstr *out);
int cstr_reverse(cstr *self);
int cstr_remove(cstr *self, size_t start, size_t len);
int cstr_split(cstr *self, const char *delim, cstr **out, size_t *count);
int cstr_clear(cstr *self);
int cstr_free(cstr *self);