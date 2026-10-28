/**
 * @file dsu.h
 * @brief Disjoint Set Union (Union-Find) with path compression and union by rank in C11.
 */

#ifndef DS_DSU_H
#define DS_DSU_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t *parent;     /**< Array of parent pointers (root points to itself) */
    size_t *rank;       /**< Approximate tree depth used to balance unions */
    size_t num_elements;/**< Total number of elements in the universe [0..n-1] */
    size_t num_sets;    /**< Current count of disjoint sets */
} DSU;

/**
 * @brief Allocates and initializes DSU universe with elements 0 to n-1.
 */
ds_status_t dsu_init(DSU *dsu, size_t n);

/**
 * @brief Frees internal parent and rank arrays.
 */
void dsu_destroy(DSU *dsu);

/**
 * @brief Finds the canonical representative of x with two-pass path compression.
 * @param out_root Output parameter receiving root index.
 */
ds_status_t dsu_find(DSU *dsu, size_t x, size_t *out_root);

/**
 * @brief Merges sets containing x and y using union by rank.
 * @param merged Set to true if distinct sets were merged, false if already connected.
 */
ds_status_t dsu_union(DSU *dsu, size_t x, size_t y, bool *merged);

/**
 * @brief Checks if x and y belong to the same set.
 */
bool dsu_connected(DSU *dsu, size_t x, size_t y);

/**
 * @brief Returns current number of disjoint sets.
 */
size_t dsu_set_count(const DSU *dsu);

#ifdef __cplusplus
}
#endif

#endif // DS_DSU_H
