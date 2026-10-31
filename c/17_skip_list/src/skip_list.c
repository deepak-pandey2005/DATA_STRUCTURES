#include "skip_list.h"
#include <stdlib.h>
#include <string.h>

static skiplist_node_t *create_node(int level, int key, int value) {
    skiplist_node_t *node = (skiplist_node_t *)malloc(
        sizeof(skiplist_node_t) + (size_t)level * sizeof(skiplist_node_t *)
    );
    if (!node) return NULL;
    node->key = key;
    node->value = value;
    node->level = level;
    for (int i = 0; i < level; ++i) {
        node->forward[i] = NULL;
    }
    return node;
}

static int random_level(void) {
    int lvl = 1;
    while ((rand() & 1) && lvl < SKIPLIST_MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

ds_status_t skiplist_create(skiplist_t **out_list) {
    if (!out_list) return DS_ERR_INVALID_ARG;

    skiplist_t *list = (skiplist_t *)malloc(sizeof(skiplist_t));
    if (!list) return DS_ERR_OUT_OF_MEMORY;

    list->max_level = SKIPLIST_MAX_LEVEL;
    list->level = 1;
    list->size = 0;

    list->head = create_node(SKIPLIST_MAX_LEVEL, 0, 0);
    if (!list->head) {
        free(list);
        return DS_ERR_OUT_OF_MEMORY;
    }

    *out_list = list;
    return DS_SUCCESS;
}

void skiplist_destroy(skiplist_t *list) {
    if (!list) return;

    skiplist_node_t *curr = list->head->forward[0];
    while (curr) {
        skiplist_node_t *next = curr->forward[0];
        free(curr);
        curr = next;
    }
    free(list->head);
    free(list);
}

ds_status_t skiplist_search(const skiplist_t *list, int key, int *out_value) {
    if (!list) return DS_ERR_INVALID_ARG;

    skiplist_node_t *curr = list->head;
    for (int i = list->level - 1; i >= 0; --i) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];
    if (curr && curr->key == key) {
        if (out_value) *out_value = curr->value;
        return DS_SUCCESS;
    }
    return DS_ERR_NOT_FOUND;
}

ds_status_t skiplist_insert(skiplist_t *list, int key, int value) {
    if (!list) return DS_ERR_INVALID_ARG;

    skiplist_node_t *update[SKIPLIST_MAX_LEVEL];
    skiplist_node_t *curr = list->head;

    for (int i = list->level - 1; i >= 0; --i) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];
    if (curr && curr->key == key) {
        curr->value = value;
        return DS_SUCCESS;
    }

    int new_level = random_level();
    if (new_level > list->level) {
        for (int i = list->level; i < new_level; ++i) {
            update[i] = list->head;
        }
        list->level = new_level;
    }

    skiplist_node_t *new_node = create_node(new_level, key, value);
    if (!new_node) return DS_ERR_OUT_OF_MEMORY;

    for (int i = 0; i < new_level; ++i) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    list->size++;
    return DS_SUCCESS;
}

ds_status_t skiplist_remove(skiplist_t *list, int key) {
    if (!list) return DS_ERR_INVALID_ARG;

    skiplist_node_t *update[SKIPLIST_MAX_LEVEL];
    skiplist_node_t *curr = list->head;

    for (int i = list->level - 1; i >= 0; --i) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];
    if (!curr || curr->key != key) {
        return DS_ERR_NOT_FOUND;
    }

    for (int i = 0; i < list->level; ++i) {
        if (update[i]->forward[i] != curr) {
            break;
        }
        update[i]->forward[i] = curr->forward[i];
    }

    free(curr);

    while (list->level > 1 && list->head->forward[list->level - 1] == NULL) {
        list->level--;
    }

    list->size--;
    return DS_SUCCESS;
}

size_t skiplist_size(const skiplist_t *list) {
    return list ? list->size : 0;
}

bool skiplist_is_empty(const skiplist_t *list) {
    return list ? list->size == 0 : true;
}
