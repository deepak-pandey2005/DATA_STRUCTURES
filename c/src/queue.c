#include "queue.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUEUE_CAPACITY 8

ds_status_t queue_init(Queue *q, size_t elem_size, size_t initial_cap) {
    if (!q || elem_size == 0) return DS_ERR_NULL_PTR;

    q->elem_size = elem_size;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
    q->capacity = (initial_cap > 0) ? initial_cap : DEFAULT_QUEUE_CAPACITY;
    q->data = malloc(q->capacity * elem_size);
    if (!q->data) {
        q->capacity = 0;
        return DS_ERR_OUT_OF_MEMORY;
    }
    return DS_SUCCESS;
}

static ds_status_t queue_grow(Queue *q) {
    size_t new_cap = q->capacity * 2;
    void *new_data = malloc(new_cap * q->elem_size);
    if (!new_data) return DS_ERR_OUT_OF_MEMORY;

    char *dst = (char *)new_data;
    const char *src = (const char *)q->data;

    for (size_t i = 0; i < q->size; i++) {
        size_t idx = (q->head + i) % q->capacity;
        memcpy(dst + (i * q->elem_size), src + (idx * q->elem_size), q->elem_size);
    }

    free(q->data);
    q->data = new_data;
    q->head = 0;
    q->tail = q->size;
    q->capacity = new_cap;
    return DS_SUCCESS;
}

ds_status_t queue_enqueue(Queue *q, const void *elem) {
    if (!q || !elem) return DS_ERR_NULL_PTR;

    if (q->size >= q->capacity) {
        ds_status_t st = queue_grow(q);
        if (st != DS_SUCCESS) return st;
    }

    char *dst = (char *)q->data + (q->tail * q->elem_size);
    memcpy(dst, elem, q->elem_size);
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return DS_SUCCESS;
}

ds_status_t queue_dequeue(Queue *q, void *out_elem) {
    if (!q) return DS_ERR_NULL_PTR;
    if (q->size == 0) return DS_ERR_EMPTY;

    if (out_elem) {
        const char *src = (const char *)q->data + (q->head * q->elem_size);
        memcpy(out_elem, src, q->elem_size);
    }

    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return DS_SUCCESS;
}

void *queue_peek(const Queue *q) {
    if (!q || q->size == 0) return NULL;
    return (char *)q->data + (q->head * q->elem_size);
}

size_t queue_size(const Queue *q) {
    return q ? q->size : 0;
}

bool queue_is_empty(const Queue *q) {
    return (!q || q->size == 0);
}

void queue_clear(Queue *q) {
    if (q) {
        q->head = 0;
        q->tail = 0;
        q->size = 0;
    }
}

void queue_free(Queue *q) {
    if (q) {
        free(q->data);
        q->data = NULL;
        q->head = 0;
        q->tail = 0;
        q->size = 0;
        q->capacity = 0;
    }
}
