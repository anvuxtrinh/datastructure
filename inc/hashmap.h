#pragma once

typedef void *any_t;

struct hashmap_node {
    any_t key;
    any_t value;
    struct hashmap_node *next;
};

typedef struct hashmap {
    int size;
    int cap;
    struct hashmap_node **buckets;
} HashMap;

int hashmap_put(HashMap *self, any_t key, any_t value);
any_t hashmap_get(HashMap *self, any_t key);
void hashmap_free(HashMap *self);
