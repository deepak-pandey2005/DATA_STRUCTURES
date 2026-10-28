/**
 * @file dsu.c
 * @brief Implementation of Disjoint Set Union in C11.
 */

#include "dsu.h"
#include <stdlib.h>

ds_status_t dsu_init(DSU *dsu, size_t n) {
    if (!dsu || n == 0) return DS_ERR_INVALID_ARG;

    dsu->parent = malloc(n * sizeof(size_t));
    dsu->rank = calloc(n, sizeof(size_t));
    if (!dsu->parent || !dsu->rank) {
        free(dsu->parent);
        free(dsu->rank);
        dsu->parent = NULL;
        dsu->rank = NULL;
        return DS_ERR_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < n; i++) {
        dsu->parent[i] = i; // Every element is initially its own root
    }

    dsu->num_elements = n;
    dsu->num_sets = n;
    return DS_SUCCESS;
}

void dsu_destroy(DSU *dsu) {
    if (!dsu) return;
    free(dsu->parent);
    free(dsu->rank);
    dsu->parent = NULL;
    dsu->rank = NULL;
    dsu->num_elements = 0;
    dsu->num_sets = 0;
}

ds_status_t dsu_find(DSU *dsu, size_t x, size_t *out_root) {
    if (!dsu || !dsu->parent || !out_root) return DS_ERR_NULL_PTR;
    if (x >= dsu->num_elements) return DS_ERR_OUT_OF_BOUNDS;

    // Path compression: flatten tree iteratively or recursively
    size_t root = x;
    while (root != dsu->parent[root]) {
        root = dsu->parent[root];
    }

    // Path compression pass
    size_t curr = x;
    while (curr != root) {
        size_t next = dsu->parent[curr];
        dsu->parent[curr] = root;
        curr = next;
    }

    *out_root = root;
    return DS_SUCCESS;
}

ds_status_t dsu_union(DSU *dsu, size_t x, size_t y, bool *merged) {
    if (!dsu || !dsu->parent) return DS_ERR_NULL_PTR;
    if (x >= dsu->num_elements || y >= dsu->num_elements) return DS_ERR_OUT_OF_BOUNDS;

    size_t root_x = 0;
    size_t root_y = 0;
    dsu_find(dsu, x, &root_x);
    dsu_find(dsu, y, &root_y);

    if (root_x == root_y) {
        if (merged) *merged = false;
        return DS_SUCCESS; // Already in the same set
    }

    // Union by rank
    if (dsu->rank[root_x] < dsu->rank[root_y]) {
        dsu->parent[root_x] = root_y;
    } else if (dsu->rank[root_x] > dsu->rank[root_y]) {
        dsu->parent[root_y] = root_x;
    } else {
        dsu->parent[root_y] = root_x;
        dsu->rank[root_x]++;
    }

    dsu->num_sets--;
    if (merged) *merged = true;
    return DS_SUCCESS;
}

bool dsu_connected(DSU *dsu, size_t x, size_t y) {
    if (!dsu || x >= dsu->num_elements || y >= dsu->num_elements) return false;
    size_t root_x = 0;
    size_t root_y = 0;
    if (dsu_find(dsu, x, &root_x) != DS_SUCCESS) return false;
    if (dsu_find(dsu, y, &root_y) != DS_SUCCESS) return false;
    return root_x == root_y;
}

size_t dsu_set_count(const DSU *dsu) {
    return dsu ? dsu->num_sets : 0;
}

