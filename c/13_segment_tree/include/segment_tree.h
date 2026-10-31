/**
 * @file segment_tree.h
 * @brief Segment Tree for range sum queries and point updates in C11.
 */

#ifndef DS_SEGMENT_TREE_H
#define DS_SEGMENT_TREE_H

#include "common.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int64_t *tree;     /**< Contiguous tree array of size up to 4*n */
    size_t n;          /**< Number of elements in original array */
} SegmentTree;

/**
 * @brief Builds a Segment Tree from an existing array.
 */
ds_status_t segtree_init(SegmentTree *st, const int64_t *arr, size_t n);

/**
 * @brief Destroys and frees allocated segment tree memory.
 */
void segtree_destroy(SegmentTree *st);

/**
 * @brief Queries the sum in range [left, right] inclusive (0-based indexing).
 */
ds_status_t segtree_query(const SegmentTree *st, size_t left, size_t right, int64_t *out_sum);

/**
 * @brief Updates value at index (0-based) to new_val in O(log n).
 */
ds_status_t segtree_update(SegmentTree *st, size_t index, int64_t new_val);

/**
 * @brief Returns number of elements in the tree range.
 */
size_t segtree_size(const SegmentTree *st);

#ifdef __cplusplus
}
#endif

#endif // DS_SEGMENT_TREE_H
