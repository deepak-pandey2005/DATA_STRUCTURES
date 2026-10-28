#include "doubly_linked_list.h"
#include <stdlib.h>

ds_status_t dlist_init(DoublyLinkedList *list) {
    if (!list) return DS_ERR_NULL_PTR;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return DS_SUCCESS;
}

ds_status_t dlist_push_front(DoublyLinkedList *list, int value) {
    if (!list) return DS_ERR_NULL_PTR;

    DListNode *node = malloc(sizeof(DListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->prev = NULL;
    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }

    list->head = node;
    list->size++;
    return DS_SUCCESS;
}

ds_status_t dlist_push_back(DoublyLinkedList *list, int value) {
    if (!list) return DS_ERR_NULL_PTR;

    DListNode *node = malloc(sizeof(DListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    list->size++;
    return DS_SUCCESS;
}

ds_status_t dlist_pop_front(DoublyLinkedList *list, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (list->size == 0) return DS_ERR_EMPTY;

    DListNode *old_head = list->head;
    if (out_value) {
        *out_value = old_head->value;
    }

    list->head = old_head->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(old_head);
    list->size--;
    return DS_SUCCESS;
}

ds_status_t dlist_pop_back(DoublyLinkedList *list, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (list->size == 0) return DS_ERR_EMPTY;

    DListNode *old_tail = list->tail;
    if (out_value) {
        *out_value = old_tail->value;
    }

    list->tail = old_tail->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(old_tail);
    list->size--;
    return DS_SUCCESS;
}

ds_status_t dlist_insert_at(DoublyLinkedList *list, size_t index, int value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (index > list->size) return DS_ERR_OUT_OF_BOUNDS;

    if (index == 0) return dlist_push_front(list, value);
    if (index == list->size) return dlist_push_back(list, value);

    DListNode *curr;
    if (index < list->size / 2) {
        curr = list->head;
        for (size_t i = 0; i < index; i++) curr = curr->next;
    } else {
        curr = list->tail;
        for (size_t i = list->size - 1; i > index; i--) curr = curr->prev;
    }

    DListNode *node = malloc(sizeof(DListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->prev = curr->prev;
    node->next = curr;

    curr->prev->next = node;
    curr->prev = node;

    list->size++;
    return DS_SUCCESS;
}

ds_status_t dlist_remove_node(DoublyLinkedList *list, DListNode *node) {
    if (!list || !node) return DS_ERR_NULL_PTR;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    free(node);
    list->size--;
    return DS_SUCCESS;
}

ds_status_t dlist_remove_at(DoublyLinkedList *list, size_t index, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (index >= list->size) return DS_ERR_OUT_OF_BOUNDS;

    DListNode *curr;
    if (index < list->size / 2) {
        curr = list->head;
        for (size_t i = 0; i < index; i++) curr = curr->next;
    } else {
        curr = list->tail;
        for (size_t i = list->size - 1; i > index; i--) curr = curr->prev;
    }

    if (out_value) {
        *out_value = curr->value;
    }

    return dlist_remove_node(list, curr);
}

DListNode *dlist_find(const DoublyLinkedList *list, int value) {
    if (!list) return NULL;
    DListNode *curr = list->head;
    while (curr) {
        if (curr->value == value) return curr;
        curr = curr->next;
    }
    return NULL;
}

void dlist_clear(DoublyLinkedList *list) {
    if (!list) return;
    DListNode *curr = list->head;
    while (curr) {
        DListNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dlist_free(DoublyLinkedList *list) {
    dlist_clear(list);
}

size_t dlist_size(const DoublyLinkedList *list) {
    return list ? list->size : 0;
}

bool dlist_is_empty(const DoublyLinkedList *list) {
    return (!list || list->size == 0);
}
