#ifndef DS_DOUBLY_LINKED_LIST_H
#define DS_DOUBLY_LINKED_LIST_H

#include "common.h"

typedef struct DListNode {
    int value;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

typedef struct {
    DListNode *head;
    DListNode *tail;
    size_t size;
} DoublyLinkedList;

ds_status_t dlist_init(DoublyLinkedList *list);
ds_status_t dlist_push_front(DoublyLinkedList *list, int value);
ds_status_t dlist_push_back(DoublyLinkedList *list, int value);
ds_status_t dlist_pop_front(DoublyLinkedList *list, int *out_value);
ds_status_t dlist_pop_back(DoublyLinkedList *list, int *out_value);
ds_status_t dlist_insert_at(DoublyLinkedList *list, size_t index, int value);
ds_status_t dlist_remove_at(DoublyLinkedList *list, size_t index, int *out_value);
ds_status_t dlist_remove_node(DoublyLinkedList *list, DListNode *node);
DListNode *dlist_find(const DoublyLinkedList *list, int value);
void dlist_clear(DoublyLinkedList *list);
void dlist_free(DoublyLinkedList *list);
size_t dlist_size(const DoublyLinkedList *list);
bool dlist_is_empty(const DoublyLinkedList *list);

#endif /* DS_DOUBLY_LINKED_LIST_H */
