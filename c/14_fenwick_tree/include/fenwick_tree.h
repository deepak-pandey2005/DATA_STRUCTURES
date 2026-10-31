/**
 * @file fenwick_tree.h
 * @brief Binary Indexed Tree (Fenwick Tree) in C11.
 */

#ifndef DS_FENWICK_TREE_H
#define DS_FENWICK_TREE_H

#include "common.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int64_t *tree;     /**< 1-based indexed BIT array */
    size_t size;       /**< Number of elements */
} FenwickTree;

/**
 * @brief Initializes a Fenwick tree of size n with zeroes.
 */
ds_status_t fenwick_init(FenwickTree *ft, size_t n);

/**
 * @brief Initializes a Fenwick tree from an existing array in O(n).
 */
ds_status_t fenwick_from_array(FenwickTree *ft, const int64_t *arr, size_t n);

/**
 * @brief Frees memory associated with Fenwick tree.
 */
void fenwick_destroy(FenwickTree *ft);

/**
 * @brief Adds delta to the element at index (0-based) in O(log n).
 */
ds_status_t fenwick_add(FenwickTree *ft, size_t index, int64_t delta);

/**
 * @brief Computes prefix sum of range [0, index] inclusive in O(log n).
 */
ds_status_t fenwick_query(const FenwickTree *ft, size_t index, int64_t *out_sum);

/**
 * @brief Computes range sum of range [left, right] inclusive in O(log n).
 */
ds_status_t fenwick_range_query(const FenwickTree *ft, size_t left, size_t right, int64_t *out_sum);

/**
 * @brief Returns size of the Fenwick tree.
 */
size_t fenwick_size(const FenwickTree *ft);

#ifdef __cplusplus
}
#endif

#endif // DS_FENWICK_TREE_H
