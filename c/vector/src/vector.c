#include "vector.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_VECTOR_CAPACITY 8

ds_status_t vector_init(Vector *vec, size_t elem_size, size_t initial_cap) {
    if (!vec || elem_size == 0) return DS_ERR_NULL_PTR;

    vec->elem_size = elem_size;
    vec->size = 0;
    vec->capacity = (initial_cap > 0) ? initial_cap : DEFAULT_VECTOR_CAPACITY;
    vec->data = malloc(vec->capacity * elem_size);
    if (!vec->data) {
        vec->capacity = 0;
        return DS_ERR_OUT_OF_MEMORY;
    }
    return DS_SUCCESS;
}

ds_status_t vector_reserve(Vector *vec, size_t new_cap) {
    if (!vec) return DS_ERR_NULL_PTR;
    if (new_cap <= vec->capacity) return DS_SUCCESS;

    void *new_data = realloc(vec->data, new_cap * vec->elem_size);
    if (!new_data) return DS_ERR_OUT_OF_MEMORY;

    vec->data = new_data;
    vec->capacity = new_cap;
    return DS_SUCCESS;
}

ds_status_t vector_push_back(Vector *vec, const void *elem) {
    if (!vec || !elem) return DS_ERR_NULL_PTR;

    if (vec->size >= vec->capacity) {
        size_t next_cap = (vec->capacity == 0) ? DEFAULT_VECTOR_CAPACITY : vec->capacity * 2;
        ds_status_t st = vector_reserve(vec, next_cap);
        if (st != DS_SUCCESS) return st;
    }

    char *target = (char *)vec->data + (vec->size * vec->elem_size);
    memcpy(target, elem, vec->elem_size);
    vec->size++;
    return DS_SUCCESS;
}

ds_status_t vector_pop_back(Vector *vec, void *out_elem) {
    if (!vec) return DS_ERR_NULL_PTR;
    if (vec->size == 0) return DS_ERR_EMPTY;

    vec->size--;
    if (out_elem) {
        const char *src = (char *)vec->data + (vec->size * vec->elem_size);
        memcpy(out_elem, src, vec->elem_size);
    }
    return DS_SUCCESS;
}

void *vector_get(const Vector *vec, size_t index) {
    if (!vec || index >= vec->size) return NULL;
    return (char *)vec->data + (index * vec->elem_size);
}

ds_status_t vector_set(Vector *vec, size_t index, const void *elem) {
    if (!vec || !elem) return DS_ERR_NULL_PTR;
    if (index >= vec->size) return DS_ERR_OUT_OF_BOUNDS;

    char *target = (char *)vec->data + (index * vec->elem_size);
    memcpy(target, elem, vec->elem_size);
    return DS_SUCCESS;
}

ds_status_t vector_insert(Vector *vec, size_t index, const void *elem) {
    if (!vec || !elem) return DS_ERR_NULL_PTR;
    if (index > vec->size) return DS_ERR_OUT_OF_BOUNDS;

    if (vec->size >= vec->capacity) {
        size_t next_cap = (vec->capacity == 0) ? DEFAULT_VECTOR_CAPACITY : vec->capacity * 2;
        ds_status_t st = vector_reserve(vec, next_cap);
        if (st != DS_SUCCESS) return st;
    }

    char *base = (char *)vec->data;
    if (index < vec->size) {
        memmove(base + ((index + 1) * vec->elem_size),
                base + (index * vec->elem_size),
                (vec->size - index) * vec->elem_size);
    }

    memcpy(base + (index * vec->elem_size), elem, vec->elem_size);
    vec->size++;
    return DS_SUCCESS;
}

ds_status_t vector_remove_at(Vector *vec, size_t index, void *out_elem) {
    if (!vec) return DS_ERR_NULL_PTR;
    if (index >= vec->size) return DS_ERR_OUT_OF_BOUNDS;

    char *base = (char *)vec->data;
    if (out_elem) {
        memcpy(out_elem, base + (index * vec->elem_size), vec->elem_size);
    }

    if (index + 1 < vec->size) {
        memmove(base + (index * vec->elem_size),
                base + ((index + 1) * vec->elem_size),
                (vec->size - index - 1) * vec->elem_size);
    }

    vec->size--;
    return DS_SUCCESS;
}

ds_status_t vector_shrink_to_fit(Vector *vec) {
    if (!vec) return DS_ERR_NULL_PTR;
    if (vec->size == vec->capacity) return DS_SUCCESS;

    size_t target_cap = (vec->size > 0) ? vec->size : 1;
    void *new_data = realloc(vec->data, target_cap * vec->elem_size);
    if (!new_data) return DS_ERR_OUT_OF_MEMORY;

    vec->data = new_data;
    vec->capacity = target_cap;
    return DS_SUCCESS;
}

void vector_clear(Vector *vec) {
    if (vec) {
        vec->size = 0;
    }
}

void vector_free(Vector *vec) {
    if (vec) {
        free(vec->data);
        vec->data = NULL;
        vec->size = 0;
        vec->capacity = 0;
    }
}

size_t vector_size(const Vector *vec) {
    return vec ? vec->size : 0;
}

size_t vector_capacity(const Vector *vec) {
    return vec ? vec->capacity : 0;
}

bool vector_is_empty(const Vector *vec) {
    return (!vec || vec->size == 0);
}
