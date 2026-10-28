/**
 * @file heap.h
 * @brief Generic Binary Heap (Priority Queue) in C11.
 *
 * Implements a generic binary min/max heap using an array of bytes.
 * Elements are ordered using a user-provided comparator function.
 */

#ifndef DS_HEAP_H
#define DS_HEAP_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Comparator function pointer type.
 * Return < 0 if a should have higher priority than b (e.g. a < b for Min-Heap).
 * Return > 0 if b should have higher priority than a.
 * Return 0 if priorities are equal.
 */
typedef int (*heap_compare_fn)(const void *a, const void *b);

typedef struct {
    void *data;                  /**< Contiguous memory holding heap elements */
    size_t elem_size;            /**< Size of each element in bytes */
    size_t size;                 /**< Number of active elements */
    size_t capacity;             /**< Total allocated capacity */
    heap_compare_fn compare;     /**< Comparison function determining priority */
} BinaryHeap;

/**
 * @brief Initializes a binary heap with an element size, initial capacity, and comparator.
 */
ds_status_t heap_init(BinaryHeap *heap, size_t elem_size, size_t initial_cap, heap_compare_fn cmp);

/**
 * @brief Frees internal heap buffer and resets fields.
 */
void heap_destroy(BinaryHeap *heap);

/**
 * @brief Pushes a new element onto the heap and restores heap invariant via sift-up.
 */
ds_status_t heap_push(BinaryHeap *heap, const void *elem);

/**
 * @brief Retrieves the top element (min or max depending on comparator) without removing it.
 */
ds_status_t heap_peek(const BinaryHeap *heap, void *out_elem);

/**
 * @brief Removes the top element and copies it to out_elem, restoring heap invariant via sift-down.
 */
ds_status_t heap_pop(BinaryHeap *heap, void *out_elem);

/**
 * @brief Returns the number of elements in the heap.
 */
size_t heap_size(const BinaryHeap *heap);

/**
 * @brief Checks if the heap is empty.
 */
bool heap_is_empty(const BinaryHeap *heap);

#ifdef __cplusplus
}
#endif

#endif // DS_HEAP_H
