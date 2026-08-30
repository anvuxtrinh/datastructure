#include <stdlib.h>
#include "hashmap.h"

int hashmap_put(hashmap_t *self, any_t key, size_t ksize, any_t value) {
    if(self == NULL || key == NULL || ksize == 0) {
        return -EINVAL;
    }

    if(self->cap == 0){
        self->cap = 8;
        self->buckets = calloc(self->cap, sizeof(struct hashmap_node *));
        if (self->buckets == NULL) {
            return -ENOMEM;
        }
    }

    if (self->size >= self->cap * 0.75) {
        hashmap_rehash(self);
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    // Update value if key already exists
    while (cur) {
        if (cur->ksize == ksize && memcmp(cur->key, key, ksize) == 0) {
            cur->value = value;
            return 0;
        }
        cur = cur->next;
    }

    // Create a new node
    struct hashmap_node *new_node = malloc(sizeof(struct hashmap_node));
    new_node->key = key;
    new_node->ksize = ksize;
    new_node->value = value;
    new_node->next = self->buckets[idx];
    self->buckets[idx] = new_node;
    self->size++;
    return 0;
}

any_t hashmap_get(hashmap_t *self, any_t key, size_t ksize) {
    if(self == NULL || key == NULL || ksize == 0) {
        return NULL;
    }

    if(self->cap == 0 || self->buckets == NULL) {
        return NULL;
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    while (cur) {
        if (cur->ksize == ksize && memcmp(cur->key, key, ksize) == 0) {
            return cur->value;
        }
        cur = cur->next;
    }
    return NULL; // Key not found
}

void hashmap_free(hashmap_t *self) {
    if (self == NULL) {
        return;
    }

    for (int i = 0; i < self->cap; i++) {
        struct hashmap_node *node = self->buckets[i];
        while (node) {
            struct hashmap_node *next = node->next;
            free(node);
            node = next;
        }
    }
    free(self->buckets);
    self->size = 0;
    self->cap = 0;
    self->buckets = NULL;
}

unsigned int hashmap_hash(any_t key, size_t ksize) {
    if(key == NULL || ksize == 0) {
        return 0;
    }

    unsigned long hash = 5381;
    unsigned char *str = (unsigned char *)key;
    int c;
    for (size_t i = 0; i < ksize; i++) {
        c = str[i];
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

void hashmap_rehash(hashmap_t *self) {
    (void)self;
}