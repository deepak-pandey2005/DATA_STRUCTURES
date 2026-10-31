/**
 * @file fenwick_tree.c
 * @brief Implementation of Fenwick Tree (Binary Indexed Tree) in C11.
 */

#include "fenwick_tree.h"
#include <stdlib.h>

ds_status_t fenwick_init(FenwickTree *ft, size_t n) {
    if (!ft || n == 0) return DS_ERR_INVALID_ARG;

    ft->tree = calloc(n + 1, sizeof(int64_t));
    if (!ft->tree) return DS_ERR_OUT_OF_MEMORY;

    ft->size = n;
    return DS_SUCCESS;
}

ds_status_t fenwick_from_array(FenwickTree *ft, const int64_t *arr, size_t n) {
    if (!ft || !arr || n == 0) return DS_ERR_INVALID_ARG;

    ds_status_t status = fenwick_init(ft, n);
    if (status != DS_SUCCESS) return status;

    for (size_t i = 0; i < n; i++) {
        fenwick_add(ft, i, arr[i]);
    }
    return DS_SUCCESS;
}

void fenwick_destroy(FenwickTree *ft) {
    if (!ft) return;
    free(ft->tree);
    ft->tree = NULL;
    ft->size = 0;
}

ds_status_t fenwick_add(FenwickTree *ft, size_t index, int64_t delta) {
    if (!ft || !ft->tree) return DS_ERR_NULL_PTR;
    if (index >= ft->size) return DS_ERR_OUT_OF_BOUNDS;

    // Convert 0-based to 1-based index
    for (size_t i = index + 1; i <= ft->size; i += (i & (-i))) {
        ft->tree[i] += delta;
    }
    return DS_SUCCESS;
}

ds_status_t fenwick_query(const FenwickTree *ft, size_t index, int64_t *out_sum) {
    if (!ft || !ft->tree || !out_sum) return DS_ERR_NULL_PTR;
    if (index >= ft->size) return DS_ERR_OUT_OF_BOUNDS;

    int64_t sum = 0;
    for (size_t i = index + 1; i > 0; i -= (i & (-i))) {
        sum += ft->tree[i];
    }
    *out_sum = sum;
    return DS_SUCCESS;
}

ds_status_t fenwick_range_query(const FenwickTree *ft, size_t left, size_t right, int64_t *out_sum) {
    if (!ft || !ft->tree || !out_sum) return DS_ERR_NULL_PTR;
    if (left > right || right >= ft->size) return DS_ERR_OUT_OF_BOUNDS;

    int64_t sum_r = 0;
    int64_t sum_l = 0;

    ds_status_t status = fenwick_query(ft, right, &sum_r);
    if (status != DS_SUCCESS) return status;

    if (left > 0) {
        status = fenwick_query(ft, left - 1, &sum_l);
        if (status != DS_SUCCESS) return status;
    }

    *out_sum = sum_r - sum_l;
    return DS_SUCCESS;
}

size_t fenwick_size(const FenwickTree *ft) {
    return ft ? ft->size : 0;
}
