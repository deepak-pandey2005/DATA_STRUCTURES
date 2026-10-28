#include "stack.h"

ds_status_t stack_init(Stack *stack, size_t elem_size, size_t initial_cap) {
    if (!stack) return DS_ERR_NULL_PTR;
    return vector_init(&stack->vec, elem_size, initial_cap);
}

ds_status_t stack_push(Stack *stack, const void *elem) {
    if (!stack) return DS_ERR_NULL_PTR;
    return vector_push_back(&stack->vec, elem);
}

ds_status_t stack_pop(Stack *stack, void *out_elem) {
    if (!stack) return DS_ERR_NULL_PTR;
    return vector_pop_back(&stack->vec, out_elem);
}

void *stack_peek(const Stack *stack) {
    if (!stack || vector_is_empty(&stack->vec)) return NULL;
    return vector_get(&stack->vec, stack->vec.size - 1);
}

size_t stack_size(const Stack *stack) {
    return stack ? vector_size(&stack->vec) : 0;
}

bool stack_is_empty(const Stack *stack) {
    return !stack || vector_is_empty(&stack->vec);
}

void stack_clear(Stack *stack) {
    if (stack) vector_clear(&stack->vec);
}

void stack_free(Stack *stack) {
    if (stack) vector_free(&stack->vec);
}
