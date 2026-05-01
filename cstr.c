#include "cstr.h"

void s8_create_by_cptr(s8* str, const char* cptr) {
    size_t len = strlen(cptr);
    str->data = malloc(len + 1);
    assert(str->data != NULL);
    memcpy(str->data, cptr, len);
    str->data[len] = '\0';
    str->size = len;
    str->capacity = len + 1;
}

void s8_create_by_another(s8* str, const s8* src) {
    str->data = malloc(src->capacity);
    assert(str->data != NULL);
    memcpy(str->data, src->data, src->size);
    str->data[src->size] = '\0';
    str->size = src->size;
    str->capacity = src->capacity;
}

void s8_append(s8* str, const char* s){
    size_t len = strlen(s);
    vector_appendn(str, s, len);
}

int s8_to_int(s8 str){
    int num = 0;
    for(size_t i=0; i<str.size; i++){
        if(str.data[i] < '0' || str.data[i] > '9') break;
        num = num * 10 + (str.data[i] - '0');
    }
    return num;
}

void s8_free(s8* str){
    if(str->data) {
        free(str->data);
        str->data = NULL;
    }
    str->size = 0;
    str->capacity = 0;
}