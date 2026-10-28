/**
 * @file hash_table.h
 * @brief Hash Table with separate chaining in C11.
 *
 * Provides an associative key-value map with string keys and arbitrary pointer values,
 * utilizing the 64-bit FNV-1a hash algorithm and dynamic resizing.
 */

#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HashNode {
    char *key;              /**< Null-terminated heap-allocated string key */
    void *value;            /**< Generic pointer to associated payload */
    struct HashNode *next;  /**< Pointer to next node in collision chain */
} HashNode;

typedef struct {
    HashNode **buckets;     /**< Array of pointers to bucket chain heads */
    size_t capacity;        /**< Total number of allocated buckets */
    size_t size;            /**< Number of active key-value pairs stored */
} HashTable;

/**
 * @brief Computes 64-bit FNV-1a hash for a given null-terminated string.
 */
uint64_t ht_hash_fnv1a(const char *key);

/**
 * @brief Initializes a hash table with an initial bucket capacity.
 */
ds_status_t ht_init(HashTable *ht, size_t initial_capacity);

/**
 * @brief Frees all key-value nodes and internal bucket storage.
 * @param free_value_fn Optional callback to free stored value payloads (can be NULL).
 */
void ht_destroy(HashTable *ht, void (*free_value_fn)(void *));

/**
 * @brief Inserts or updates a key-value entry. If key exists, updates value.
 */
ds_status_t ht_insert(HashTable *ht, const char *key, void *value);

/**
 * @brief Retrieves the value associated with key.
 * @param out_val Receives the pointer to stored value if found.
 */
ds_status_t ht_get(const HashTable *ht, const char *key, void **out_val);

/**
 * @brief Checks if a given key exists in the hash table.
 */
bool ht_contains(const HashTable *ht, const char *key);

/**
 * @brief Removes an entry matching key.
 * @param out_val Optional pointer receiving removed value payload (can be NULL).
 */
ds_status_t ht_remove(HashTable *ht, const char *key, void **out_val);

/**
 * @brief Returns total number of key-value pairs in the table.
 */
size_t ht_size(const HashTable *ht);

/**
 * @brief Returns true if table is empty.
 */
bool ht_is_empty(const HashTable *ht);

#ifdef __cplusplus
}
#endif

#endif // DS_HASH_TABLE_H

