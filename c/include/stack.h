#ifndef DS_STACK_H
#define DS_STACK_H

#include "vector.h"

typedef struct {
    Vector vec;
} Stack;

ds_status_t stack_init(Stack *stack, size_t elem_size, size_t initial_cap);
ds_status_t stack_push(Stack *stack, const void *elem);
ds_status_t stack_pop(Stack *stack, void *out_elem);
void *stack_peek(const Stack *stack);
size_t stack_size(const Stack *stack);
bool stack_is_empty(const Stack *stack);
void stack_clear(Stack *stack);
void stack_free(Stack *stack);

#endif /* DS_STACK_H */
