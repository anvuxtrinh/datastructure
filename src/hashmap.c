#include <stdlib.h>
#include "hashmap.h"

void hashmap_rehash(HashMap *self);
unsigned int hashmap_hash(any_t key);

int hashmap_put(HashMap *self, any_t key, any_t value) {
    if (self->size >= self->cap * 0.75) {
        hashmap_rehash(self);
    }

    unsigned int idx = hashmap_hash(key) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    // Update value if key already exists
    while (cur) {
        if (cur->key == key) {
            cur->value = value;
            return 0;
        }
        cur = cur->next;
    }

    // Create a new node
    struct hashmap_node *new_node = malloc(sizeof(struct hashmap_node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = self->buckets[idx];
    self->buckets[idx] = new_node;
    self->size++;
    return 0;
}

void hashmap_free(HashMap *self) {
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