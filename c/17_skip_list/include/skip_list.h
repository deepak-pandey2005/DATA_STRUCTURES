#ifndef DS_SKIP_LIST_H
#define DS_SKIP_LIST_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#define SKIPLIST_MAX_LEVEL 16

typedef struct skiplist_node {
    int key;
    int value;
    int level;
    struct skiplist_node *forward[];
} skiplist_node_t;

typedef struct skiplist {
    int max_level;
    int level;
    size_t size;
    skiplist_node_t *head;
} skiplist_t;

ds_status_t skiplist_create(skiplist_t **out_list);
void skiplist_destroy(skiplist_t *list);

ds_status_t skiplist_insert(skiplist_t *list, int key, int value);
ds_status_t skiplist_search(const skiplist_t *list, int key, int *out_value);
ds_status_t skiplist_remove(skiplist_t *list, int key);
size_t skiplist_size(const skiplist_t *list);
bool skiplist_is_empty(const skiplist_t *list);

#endif /* DS_SKIP_LIST_H */
