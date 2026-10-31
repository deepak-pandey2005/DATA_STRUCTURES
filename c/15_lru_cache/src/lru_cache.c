#include "lru_cache.h"
#include <stdlib.h>
#include <string.h>

typedef struct hash_entry {
    int key;
    lru_node_t *node;
    struct hash_entry *next;
} hash_entry_t;

struct lru_cache_impl {
    size_t capacity;
    size_t size;
    lru_node_t *head;
    lru_node_t *tail;
    hash_entry_t **buckets;
    size_t bucket_count;
};

static inline size_t hash_int(int key, size_t bucket_count) {
    unsigned int x = (unsigned int)key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (size_t)(x % bucket_count);
}

static void remove_node(lru_node_t *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void add_to_head(lru_cache_t *cache, lru_node_t *node) {
    node->next = cache->head->next;
    node->prev = cache->head;
    cache->head->next->prev = node;
    cache->head->next = node;
}

static void move_to_head(lru_cache_t *cache, lru_node_t *node) {
    remove_node(node);
    add_to_head(cache, node);
}

static hash_entry_t *find_entry(hash_entry_t **buckets, size_t bucket_count, int key) {
    if (!buckets || bucket_count == 0) return NULL;
    size_t idx = hash_int(key, bucket_count);
    hash_entry_t *curr = buckets[idx];
    while (curr) {
        if (curr->key == key) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

static bool insert_entry(hash_entry_t **buckets, size_t bucket_count, int key, lru_node_t *node) {
    size_t idx = hash_int(key, bucket_count);
    hash_entry_t *entry = (hash_entry_t *)malloc(sizeof(hash_entry_t));
    if (!entry) return false;
    entry->key = key;
    entry->node = node;
    entry->next = buckets[idx];
    buckets[idx] = entry;
    return true;
}

static void delete_entry(hash_entry_t **buckets, size_t bucket_count, int key) {
    size_t idx = hash_int(key, bucket_count);
    hash_entry_t *curr = buckets[idx];
    hash_entry_t *prev = NULL;
    while (curr) {
        if (curr->key == key) {
            if (prev) {
                prev->next = curr->next;
            } else {
                buckets[idx] = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

ds_status_t lru_create(size_t capacity, lru_cache_t **out_cache) {
    if (!out_cache || capacity == 0) {
        return DS_ERR_INVALID_ARG;
    }

    lru_cache_t *c = (lru_cache_t *)malloc(sizeof(lru_cache_t));
    if (!c) return DS_ERR_OUT_OF_MEMORY;

    c->capacity = capacity;
    c->size = 0;
    c->map_cap = capacity * 2 + 7;
    c->map = (lru_node_t **)calloc(c->map_cap, sizeof(hash_entry_t *));
    if (!c->map) {
        free(c);
        return DS_ERR_OUT_OF_MEMORY;
    }

    c->head = (lru_node_t *)malloc(sizeof(lru_node_t));
    c->tail = (lru_node_t *)malloc(sizeof(lru_node_t));
    if (!c->head || !c->tail) {
        free(c->head);
        free(c->tail);
        free(c->map);
        free(c);
        return DS_ERR_OUT_OF_MEMORY;
    }

    c->head->prev = NULL;
    c->head->next = c->tail;
    c->tail->prev = c->head;
    c->tail->next = NULL;

    *out_cache = c;
    return DS_SUCCESS;
}

void lru_destroy(lru_cache_t *cache) {
    if (!cache) return;

    lru_node_t *curr = cache->head;
    while (curr) {
        lru_node_t *next = curr->next;
        free(curr);
        curr = next;
    }

    hash_entry_t **buckets = (hash_entry_t **)cache->map;
    if (buckets) {
        for (size_t i = 0; i < cache->map_cap; ++i) {
            hash_entry_t *entry = buckets[i];
            while (entry) {
                hash_entry_t *next_entry = entry->next;
                free(entry);
                entry = next_entry;
            }
        }
        free(buckets);
    }

    free(cache);
}

ds_status_t lru_get(lru_cache_t *cache, int key, int *out_value) {
    if (!cache || !out_value) return DS_ERR_INVALID_ARG;

    hash_entry_t **buckets = (hash_entry_t **)cache->map;
    hash_entry_t *entry = find_entry(buckets, cache->map_cap, key);
    if (!entry) {
        return DS_ERR_NOT_FOUND;
    }

    *out_value = entry->node->value;
    move_to_head(cache, entry->node);
    return DS_SUCCESS;
}

ds_status_t lru_put(lru_cache_t *cache, int key, int value) {
    if (!cache) return DS_ERR_INVALID_ARG;

    hash_entry_t **buckets = (hash_entry_t **)cache->map;
    hash_entry_t *entry = find_entry(buckets, cache->map_cap, key);

    if (entry) {
        entry->node->value = value;
        move_to_head(cache, entry->node);
        return DS_SUCCESS;
    }

    if (cache->size >= cache->capacity) {
        /* Evict LRU node (tail->prev) */
        lru_node_t *lru = cache->tail->prev;
        delete_entry(buckets, cache->map_cap, lru->key);
        remove_node(lru);
        free(lru);
        cache->size--;
    }

    lru_node_t *new_node = (lru_node_t *)malloc(sizeof(lru_node_t));
    if (!new_node) return DS_ERR_OUT_OF_MEMORY;
    new_node->key = key;
    new_node->value = value;

    if (!insert_entry(buckets, cache->map_cap, key, new_node)) {
        free(new_node);
        return DS_ERR_OUT_OF_MEMORY;
    }

    add_to_head(cache, new_node);
    cache->size++;

    return DS_SUCCESS;
}

size_t lru_size(const lru_cache_t *cache) {
    return cache ? cache->size : 0;
}

size_t lru_capacity(const lru_cache_t *cache) {
    return cache ? cache->capacity : 0;
}
