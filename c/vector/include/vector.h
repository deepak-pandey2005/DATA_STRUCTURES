#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "common.h"

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} Vector;

ds_status_t vector_init(Vector *vec, size_t elem_size, size_t initial_cap);
ds_status_t vector_push_back(Vector *vec, const void *elem);
ds_status_t vector_pop_back(Vector *vec, void *out_elem);
void *vector_get(const Vector *vec, size_t index);
ds_status_t vector_set(Vector *vec, size_t index, const void *elem);
ds_status_t vector_insert(Vector *vec, size_t index, const void *elem);
ds_status_t vector_remove_at(Vector *vec, size_t index, void *out_elem);
ds_status_t vector_reserve(Vector *vec, size_t new_cap);
ds_status_t vector_shrink_to_fit(Vector *vec);
void vector_clear(Vector *vec);
void vector_free(Vector *vec);
size_t vector_size(const Vector *vec);
size_t vector_capacity(const Vector *vec);
bool vector_is_empty(const Vector *vec);

#endif /* DS_VECTOR_H */
