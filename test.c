#include <stdio.h>
#include "vector.h"

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} VecNum;

int main(int argc, char *argv[]) {
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
    return 0;
}