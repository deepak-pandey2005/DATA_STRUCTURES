#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct lru_node {
    int key;
    int value;
    struct lru_node *prev;
    struct lru_node *next;
} lru_node_t;

typedef struct lru_cache {
    size_t capacity;
    size_t size;
    lru_node_t *head; /* Sentinel head (MRU is head->next) */
    lru_node_t *tail; /* Sentinel tail (LRU is tail->prev) */
    lru_node_t **map; /* Hash table buckets (linear probing or separate bucket lists) */
    size_t map_cap;
} lru_cache_t;

ds_status_t lru_create(size_t capacity, lru_cache_t **out_cache);
void lru_destroy(lru_cache_t *cache);

ds_status_t lru_get(lru_cache_t *cache, int key, int *out_value);
ds_status_t lru_put(lru_cache_t *cache, int key, int value);
size_t lru_size(const lru_cache_t *cache);
size_t lru_capacity(const lru_cache_t *cache);

#endif /* DS_LRU_CACHE_H */
