#ifndef DS_SINGLY_LINKED_LIST_H
#define DS_SINGLY_LINKED_LIST_H

#include "common.h"

typedef struct SListNode {
    int value;
    struct SListNode *next;
} SListNode;

typedef struct {
    SListNode *head;
    SListNode *tail;
    size_t size;
} SinglyLinkedList;

ds_status_t slist_init(SinglyLinkedList *list);
ds_status_t slist_push_front(SinglyLinkedList *list, int value);
ds_status_t slist_push_back(SinglyLinkedList *list, int value);
ds_status_t slist_pop_front(SinglyLinkedList *list, int *out_value);
ds_status_t slist_pop_back(SinglyLinkedList *list, int *out_value);
ds_status_t slist_insert_at(SinglyLinkedList *list, size_t index, int value);
ds_status_t slist_remove_at(SinglyLinkedList *list, size_t index, int *out_value);
ds_status_t slist_remove_value(SinglyLinkedList *list, int value);
bool slist_contains(const SinglyLinkedList *list, int value);
ds_status_t slist_get(const SinglyLinkedList *list, size_t index, int *out_value);
void slist_reverse(SinglyLinkedList *list);
void slist_clear(SinglyLinkedList *list);
void slist_free(SinglyLinkedList *list);
size_t slist_size(const SinglyLinkedList *list);
bool slist_is_empty(const SinglyLinkedList *list);

#endif /* DS_SINGLY_LINKED_LIST_H */
