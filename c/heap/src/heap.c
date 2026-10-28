/**
 * @file heap.c
 * @brief Implementation of generic binary heap in C11.
 */

#include "heap.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HEAP_CAPACITY 8

static inline void *heap_elem_ptr(const BinaryHeap *heap, size_t index) {
    return (char *)heap->data + (index * heap->elem_size);
}

// Swap two elements in the heap buffer
static void heap_swap(BinaryHeap *heap, size_t i, size_t j) {
    if (i == j) return;
    char temp[256];
    void *pi = heap_elem_ptr(heap, i);
    void *pj = heap_elem_ptr(heap, j);

    if (heap->elem_size <= sizeof(temp)) {
        memcpy(temp, pi, heap->elem_size);
        memcpy(pi, pj, heap->elem_size);
        memcpy(pj, temp, heap->elem_size);
    } else {
        void *dyn = malloc(heap->elem_size);
        if (!dyn) return;
        memcpy(dyn, pi, heap->elem_size);
        memcpy(pi, pj, heap->elem_size);
        memcpy(pj, dyn, heap->elem_size);
        free(dyn);
    }
}

// Sifts the element at index up until heap property is restored
static void sift_up(BinaryHeap *heap, size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        // If child has higher priority than parent, swap
        if (heap->compare(heap_elem_ptr(heap, index), heap_elem_ptr(heap, parent)) < 0) {
            heap_swap(heap, index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

// Sifts the element at index down until heap property is restored
static void sift_down(BinaryHeap *heap, size_t index) {
    size_t size = heap->size;
    while (true) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t best = index;

        if (left < size && heap->compare(heap_elem_ptr(heap, left), heap_elem_ptr(heap, best)) < 0) {
            best = left;
        }
        if (right < size && heap->compare(heap_elem_ptr(heap, right), heap_elem_ptr(heap, best)) < 0) {
            best = right;
        }

        if (best != index) {
            heap_swap(heap, index, best);
            index = best;
        } else {
            break;
        }
    }
}

ds_status_t heap_init(BinaryHeap *heap, size_t elem_size, size_t initial_cap, heap_compare_fn cmp) {
    if (!heap || elem_size == 0 || !cmp) return DS_ERR_NULL_PTR;

    size_t cap = (initial_cap > 0) ? initial_cap : DEFAULT_HEAP_CAPACITY;
    heap->data = malloc(cap * elem_size);
    if (!heap->data) return DS_ERR_OUT_OF_MEMORY;

    heap->elem_size = elem_size;
    heap->size = 0;
    heap->capacity = cap;
    heap->compare = cmp;
    return DS_SUCCESS;
}

void heap_destroy(BinaryHeap *heap) {
    if (!heap) return;
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
    heap->elem_size = 0;
    heap->compare = NULL;
}

ds_status_t heap_push(BinaryHeap *heap, const void *elem) {
    if (!heap || !elem) return DS_ERR_NULL_PTR;

    if (heap->size >= heap->capacity) {
        size_t new_cap = (heap->capacity == 0) ? DEFAULT_HEAP_CAPACITY : heap->capacity * 2;
        void *new_data = realloc(heap->data, new_cap * heap->elem_size);
        if (!new_data) return DS_ERR_OUT_OF_MEMORY;
        heap->data = new_data;
        heap->capacity = new_cap;
    }

    void *dest = heap_elem_ptr(heap, heap->size);
    memcpy(dest, elem, heap->elem_size);
    sift_up(heap, heap->size);
    heap->size++;
    return DS_SUCCESS;
}

ds_status_t heap_peek(const BinaryHeap *heap, void *out_elem) {
    if (!heap || !out_elem) return DS_ERR_NULL_PTR;
    if (heap->size == 0) return DS_ERR_EMPTY;

    memcpy(out_elem, heap_elem_ptr(heap, 0), heap->elem_size);
    return DS_SUCCESS;
}

ds_status_t heap_pop(BinaryHeap *heap, void *out_elem) {
    if (!heap) return DS_ERR_NULL_PTR;
    if (heap->size == 0) return DS_ERR_EMPTY;

    if (out_elem) {
        memcpy(out_elem, heap_elem_ptr(heap, 0), heap->elem_size);
    }

    // Move last element to root
    heap->size--;
    if (heap->size > 0) {
        memcpy(heap_elem_ptr(heap, 0), heap_elem_ptr(heap, heap->size), heap->elem_size);
        sift_down(heap, 0);
    }
    return DS_SUCCESS;
}

size_t heap_size(const BinaryHeap *heap) {
    return heap ? heap->size : 0;
}

bool heap_is_empty(const BinaryHeap *heap) {
    return !heap || heap->size == 0;
}
