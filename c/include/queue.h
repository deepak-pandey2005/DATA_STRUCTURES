#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "common.h"

typedef struct {
    void *data;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
    size_t elem_size;
} Queue;

ds_status_t queue_init(Queue *q, size_t elem_size, size_t initial_cap);
ds_status_t queue_enqueue(Queue *q, const void *elem);
ds_status_t queue_dequeue(Queue *q, void *out_elem);
void *queue_peek(const Queue *q);
size_t queue_size(const Queue *q);
bool queue_is_empty(const Queue *q);
void queue_clear(Queue *q);
void queue_free(Queue *q);

#endif /* DS_QUEUE_H */
