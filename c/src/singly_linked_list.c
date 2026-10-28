#include "singly_linked_list.h"
#include <stdlib.h>

ds_status_t slist_init(SinglyLinkedList *list) {
    if (!list) return DS_ERR_NULL_PTR;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return DS_SUCCESS;
}

ds_status_t slist_push_front(SinglyLinkedList *list, int value) {
    if (!list) return DS_ERR_NULL_PTR;

    SListNode *node = malloc(sizeof(SListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->next = list->head;
    list->head = node;

    if (list->size == 0) {
        list->tail = node;
    }

    list->size++;
    return DS_SUCCESS;
}

ds_status_t slist_push_back(SinglyLinkedList *list, int value) {
    if (!list) return DS_ERR_NULL_PTR;

    SListNode *node = malloc(sizeof(SListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->next = NULL;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return DS_SUCCESS;
}

ds_status_t slist_pop_front(SinglyLinkedList *list, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (list->size == 0) return DS_ERR_EMPTY;

    SListNode *old_head = list->head;
    if (out_value) {
        *out_value = old_head->value;
    }

    list->head = old_head->next;
    free(old_head);
    list->size--;

    if (list->size == 0) {
        list->tail = NULL;
    }

    return DS_SUCCESS;
}

ds_status_t slist_pop_back(SinglyLinkedList *list, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (list->size == 0) return DS_ERR_EMPTY;

    if (list->size == 1) {
        return slist_pop_front(list, out_value);
    }

    SListNode *curr = list->head;
    while (curr->next != list->tail) {
        curr = curr->next;
    }

    if (out_value) {
        *out_value = list->tail->value;
    }

    free(list->tail);
    curr->next = NULL;
    list->tail = curr;
    list->size--;

    return DS_SUCCESS;
}

ds_status_t slist_insert_at(SinglyLinkedList *list, size_t index, int value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (index > list->size) return DS_ERR_OUT_OF_BOUNDS;

    if (index == 0) return slist_push_front(list, value);
    if (index == list->size) return slist_push_back(list, value);

    SListNode *curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    SListNode *node = malloc(sizeof(SListNode));
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->value = value;
    node->next = curr->next;
    curr->next = node;
    list->size++;

    return DS_SUCCESS;
}

ds_status_t slist_remove_at(SinglyLinkedList *list, size_t index, int *out_value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (index >= list->size) return DS_ERR_OUT_OF_BOUNDS;

    if (index == 0) return slist_pop_front(list, out_value);

    SListNode *curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    SListNode *to_remove = curr->next;
    if (out_value) {
        *out_value = to_remove->value;
    }

    curr->next = to_remove->next;
    if (to_remove == list->tail) {
        list->tail = curr;
    }

    free(to_remove);
    list->size--;
    return DS_SUCCESS;
}

ds_status_t slist_remove_value(SinglyLinkedList *list, int value) {
    if (!list) return DS_ERR_NULL_PTR;
    if (list->size == 0) return DS_ERR_NOT_FOUND;

    if (list->head->value == value) {
        return slist_pop_front(list, NULL);
    }

    SListNode *curr = list->head;
    while (curr->next && curr->next->value != value) {
        curr = curr->next;
    }

    if (!curr->next) return DS_ERR_NOT_FOUND;

    SListNode *to_remove = curr->next;
    curr->next = to_remove->next;
    if (to_remove == list->tail) {
        list->tail = curr;
    }

    free(to_remove);
    list->size--;
    return DS_SUCCESS;
}

bool slist_contains(const SinglyLinkedList *list, int value) {
    if (!list) return false;
    SListNode *curr = list->head;
    while (curr) {
        if (curr->value == value) return true;
        curr = curr->next;
    }
    return false;
}

ds_status_t slist_get(const SinglyLinkedList *list, size_t index, int *out_value) {
    if (!list || !out_value) return DS_ERR_NULL_PTR;
    if (index >= list->size) return DS_ERR_OUT_OF_BOUNDS;

    SListNode *curr = list->head;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }

    *out_value = curr->value;
    return DS_SUCCESS;
}

void slist_reverse(SinglyLinkedList *list) {
    if (!list || list->size <= 1) return;

    SListNode *prev = NULL;
    SListNode *curr = list->head;
    SListNode *next = NULL;

    list->tail = list->head;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}

void slist_clear(SinglyLinkedList *list) {
    if (!list) return;
    SListNode *curr = list->head;
    while (curr) {
        SListNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void slist_free(SinglyLinkedList *list) {
    slist_clear(list);
}

size_t slist_size(const SinglyLinkedList *list) {
    return list ? list->size : 0;
}

bool slist_is_empty(const SinglyLinkedList *list) {
    return (!list || list->size == 0);
}
