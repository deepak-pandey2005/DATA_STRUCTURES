/**
 * @file hash_table.c
 * @brief Implementation of Hash Table with separate chaining in C11.
 */

#define _POSIX_C_SOURCE 200809L
#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HT_CAPACITY 16
#define MAX_LOAD_FACTOR 0.75

// 64-bit FNV-1a hash algorithm constants
#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL

uint64_t ht_hash_fnv1a(const char *key) {
    if (!key) return 0;
    uint64_t hash = FNV_OFFSET_BASIS;
    while (*key) {
        hash ^= (uint64_t)(unsigned char)(*key);
        hash *= FNV_PRIME;
        key++;
    }
    return hash;
}

static ds_status_t ht_resize(HashTable *ht, size_t new_cap) {
    HashNode **new_buckets = calloc(new_cap, sizeof(HashNode *));
    if (!new_buckets) return DS_ERR_OUT_OF_MEMORY;

    for (size_t i = 0; i < ht->capacity; i++) {
        HashNode *curr = ht->buckets[i];
        while (curr) {
            HashNode *next = curr->next;
            size_t new_idx = ht_hash_fnv1a(curr->key) % new_cap;
            curr->next = new_buckets[new_idx];
            new_buckets[new_idx] = curr;
            curr = next;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_cap;
    return DS_SUCCESS;
}

ds_status_t ht_init(HashTable *ht, size_t initial_capacity) {
    if (!ht) return DS_ERR_NULL_PTR;

    size_t cap = (initial_capacity > 0) ? initial_capacity : DEFAULT_HT_CAPACITY;
    ht->buckets = calloc(cap, sizeof(HashNode *));
    if (!ht->buckets) return DS_ERR_OUT_OF_MEMORY;

    ht->capacity = cap;
    ht->size = 0;
    return DS_SUCCESS;
}

void ht_destroy(HashTable *ht, void (*free_value_fn)(void *)) {
    if (!ht || !ht->buckets) return;

    for (size_t i = 0; i < ht->capacity; i++) {
        HashNode *curr = ht->buckets[i];
        while (curr) {
            HashNode *next = curr->next;
            free(curr->key);
            if (free_value_fn && curr->value) {
                free_value_fn(curr->value);
            }
            free(curr);
            curr = next;
        }
    }

    free(ht->buckets);
    ht->buckets = NULL;
    ht->capacity = 0;
    ht->size = 0;
}

ds_status_t ht_insert(HashTable *ht, const char *key, void *value) {
    if (!ht || !key) return DS_ERR_NULL_PTR;

    // Check load factor before insertion
    if ((double)(ht->size + 1) / (double)ht->capacity >= MAX_LOAD_FACTOR) {
        ds_status_t res = ht_resize(ht, ht->capacity * 2);
        if (res != DS_SUCCESS) return res;
    }

    size_t idx = ht_hash_fnv1a(key) % ht->capacity;
    HashNode *curr = ht->buckets[idx];

    // Check if key already exists -> update
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            curr->value = value;
            return DS_SUCCESS;
        }
        curr = curr->next;
    }

    // Allocate new node and prepend to chain
    HashNode *node = malloc(sizeof(HashNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->key = strdup(key);
    if (!node->key) {
        free(node);
        return DS_ERR_OUT_OF_MEMORY;
    }
    node->value = value;
    node->next = ht->buckets[idx];
    ht->buckets[idx] = node;
    ht->size++;

    return DS_SUCCESS;
}

ds_status_t ht_get(const HashTable *ht, const char *key, void **out_val) {
    if (!ht || !key || !out_val) return DS_ERR_NULL_PTR;
    if (ht->size == 0) return DS_ERR_NOT_FOUND;

    size_t idx = ht_hash_fnv1a(key) % ht->capacity;
    HashNode *curr = ht->buckets[idx];

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            *out_val = curr->value;
            return DS_SUCCESS;
        }
        curr = curr->next;
    }

    return DS_ERR_NOT_FOUND;
}

bool ht_contains(const HashTable *ht, const char *key) {
    void *dummy = NULL;
    return ht_get(ht, key, &dummy) == DS_SUCCESS;
}

ds_status_t ht_remove(HashTable *ht, const char *key, void **out_val) {
    if (!ht || !key) return DS_ERR_NULL_PTR;
    if (ht->size == 0) return DS_ERR_NOT_FOUND;

    size_t idx = ht_hash_fnv1a(key) % ht->capacity;
    HashNode *curr = ht->buckets[idx];
    HashNode *prev = NULL;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                ht->buckets[idx] = curr->next;
            }

            if (out_val) {
                *out_val = curr->value;
            }

            free(curr->key);
            free(curr);
            ht->size--;
            return DS_SUCCESS;
        }
        prev = curr;
        curr = curr->next;
    }

    return DS_ERR_NOT_FOUND;
}

size_t ht_size(const HashTable *ht) {
    return ht ? ht->size : 0;
}

bool ht_is_empty(const HashTable *ht) {
    return !ht || ht->size == 0;
}
