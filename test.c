#include <stdio.h>
#include "cstr.h"
#include "vector.h"

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} VecNum;

int main(int argc, char *argv[]) {
    // Test vector
    VecNum vec;
    vector_init(&vec);
    for(int i = 0; i < 10; i++) {
        vector_append(&vec, i);
    }
    for(size_t i = 0; i < vec.size; i++) {
        printf("%d ", vec.data[i]);
    }
    printf("\n");
    vector_clear(&vec);
    vector_free(&vec);

    // Test s8
    s8 str;
    s8_create_by_cptr(&str, "Hello World");
    printf("%s\n", str.data);
    s8_free(&str);

    char *cptr = "Hello Again";
    s8 str2;
    s8_create_by_cptr(&str2, cptr);
    printf("%s\n", str2.data);
    s8_append(&str2, "!!!");
    printf("%s\n", str2.data);
    s8_free(&str2);
    return 0;
}