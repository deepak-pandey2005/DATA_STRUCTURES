/**
 * @file segment_tree.c
 * @brief Implementation of Segment Tree in C11.
 */

#include "segment_tree.h"
#include <stdlib.h>

static void build_helper(SegmentTree *st, const int64_t *arr, size_t node, size_t start, size_t end) {
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    build_helper(st, arr, left_child, start, mid);
    build_helper(st, arr, right_child, mid + 1, end);
    st->tree[node] = st->tree[left_child] + st->tree[right_child];
}

ds_status_t segtree_init(SegmentTree *st, const int64_t *arr, size_t n) {
    if (!st || !arr || n == 0) return DS_ERR_INVALID_ARG;

    st->tree = calloc(4 * n, sizeof(int64_t));
    if (!st->tree) return DS_ERR_OUT_OF_MEMORY;

    st->n = n;
    build_helper(st, arr, 0, 0, n - 1);
    return DS_SUCCESS;
}

void segtree_destroy(SegmentTree *st) {
    if (!st) return;
    free(st->tree);
    st->tree = NULL;
    st->n = 0;
}

static int64_t query_helper(const SegmentTree *st, size_t node, size_t start, size_t end, size_t l, size_t r) {
    if (r < start || end < l) {
        return 0; // Out of range identity for sum
    }
    if (l <= start && end <= r) {
        return st->tree[node];
    }
    size_t mid = start + (end - start) / 2;
    int64_t left_sum = query_helper(st, 2 * node + 1, start, mid, l, r);
    int64_t right_sum = query_helper(st, 2 * node + 2, mid + 1, end, l, r);
    return left_sum + right_sum;
}

ds_status_t segtree_query(const SegmentTree *st, size_t left, size_t right, int64_t *out_sum) {
    if (!st || !st->tree || !out_sum) return DS_ERR_NULL_PTR;
    if (left > right || right >= st->n) return DS_ERR_OUT_OF_BOUNDS;

    *out_sum = query_helper(st, 0, 0, st->n - 1, left, right);
    return DS_SUCCESS;
}

static void update_helper(SegmentTree *st, size_t node, size_t start, size_t end, size_t idx, int64_t val) {
    if (start == end) {
        st->tree[node] = val;
        return;
    }
    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    if (idx <= mid) {
        update_helper(st, left_child, start, mid, idx, val);
    } else {
        update_helper(st, right_child, mid + 1, end, idx, val);
    }
    st->tree[node] = st->tree[left_child] + st->tree[right_child];
}

ds_status_t segtree_update(SegmentTree *st, size_t index, int64_t new_val) {
    if (!st || !st->tree) return DS_ERR_NULL_PTR;
    if (index >= st->n) return DS_ERR_OUT_OF_BOUNDS;

    update_helper(st, 0, 0, st->n - 1, index, new_val);
    return DS_SUCCESS;
}

size_t segtree_size(const SegmentTree *st) {
    return st ? st->n : 0;
}
