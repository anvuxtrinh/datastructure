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

typedef struct cstr {
    char *data;
    size_t len;
    size_t cap;
} Cstr;

void cstr_init(Cstr *self);
int cstr_appendn(Cstr *self, const char *str, size_t n);
int cstr_pop(Cstr *self);
int cstr_copy(Cstr *self, const char *str, size_t n);
char cstr_at(Cstr *self, size_t index);
int cstr_substr(Cstr *self, size_t start, size_t end, Cstr *out);
int cstr_reverse(Cstr *self);
int cstr_remove(Cstr *self, size_t start, size_t len);
int cstr_split(Cstr *self, const char *delim, Cstr **out, size_t *count);
int cstr_clear(Cstr *self);
int cstr_free(Cstr *self);