#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "bst.h"

static int g_tests_passed = 0;

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "Assertion failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

#define RUN_TEST(fn) do { \
    printf("Running %s... ", #fn); \
    fn(); \
    g_tests_passed++; \
    printf("PASSED\n"); \
} while(0)

/* --- Vector Tests --- */
void test_vector() {
    Vector vec;
    TEST_ASSERT(vector_init(&vec, sizeof(int), 2) == DS_SUCCESS);
    TEST_ASSERT(vector_is_empty(&vec));

    for (int i = 0; i < 20; i++) {
        TEST_ASSERT(vector_push_back(&vec, &i) == DS_SUCCESS);
    }
    TEST_ASSERT(vector_size(&vec) == 20);

    for (size_t i = 0; i < 20; i++) {
        int *val = (int *)vector_get(&vec, i);
        TEST_ASSERT(val && *val == (int)i);
    }

    int inserted = 999;
    TEST_ASSERT(vector_insert(&vec, 5, &inserted) == DS_SUCCESS);
    TEST_ASSERT(*(int *)vector_get(&vec, 5) == 999);
    TEST_ASSERT(vector_size(&vec) == 21);

    int removed = 0;
    TEST_ASSERT(vector_remove_at(&vec, 5, &removed) == DS_SUCCESS);
    TEST_ASSERT(removed == 999);
    TEST_ASSERT(vector_size(&vec) == 20);

    int popped = 0;
    TEST_ASSERT(vector_pop_back(&vec, &popped) == DS_SUCCESS);
    TEST_ASSERT(popped == 19);
    TEST_ASSERT(vector_size(&vec) == 19);

    vector_free(&vec);
    TEST_ASSERT(vector_is_empty(&vec));
}

/* --- Singly Linked List Tests --- */
void test_slist() {
    SinglyLinkedList list;
    TEST_ASSERT(slist_init(&list) == DS_SUCCESS);

    TEST_ASSERT(slist_push_back(&list, 10) == DS_SUCCESS);
    TEST_ASSERT(slist_push_back(&list, 20) == DS_SUCCESS);
    TEST_ASSERT(slist_push_front(&list, 5) == DS_SUCCESS); // [5, 10, 20]
    TEST_ASSERT(slist_size(&list) == 3);

    int val = 0;
    TEST_ASSERT(slist_get(&list, 0, &val) == DS_SUCCESS && val == 5);
    TEST_ASSERT(slist_get(&list, 1, &val) == DS_SUCCESS && val == 10);
    TEST_ASSERT(slist_get(&list, 2, &val) == DS_SUCCESS && val == 20);

    slist_reverse(&list); // [20, 10, 5]
    TEST_ASSERT(slist_get(&list, 0, &val) == DS_SUCCESS && val == 20);
    TEST_ASSERT(slist_get(&list, 1, &val) == DS_SUCCESS && val == 10);
    TEST_ASSERT(slist_get(&list, 2, &val) == DS_SUCCESS && val == 5);

    TEST_ASSERT(slist_pop_front(&list, &val) == DS_SUCCESS && val == 20);
    TEST_ASSERT(slist_pop_back(&list, &val) == DS_SUCCESS && val == 5);
    TEST_ASSERT(slist_size(&list) == 1);

    slist_free(&list);
    TEST_ASSERT(slist_is_empty(&list));
}

/* --- Doubly Linked List Tests --- */
void test_dlist() {
    DoublyLinkedList list;
    TEST_ASSERT(dlist_init(&list) == DS_SUCCESS);

    TEST_ASSERT(dlist_push_back(&list, 100) == DS_SUCCESS);
    TEST_ASSERT(dlist_push_back(&list, 200) == DS_SUCCESS);
    TEST_ASSERT(dlist_push_front(&list, 50) == DS_SUCCESS); // [50, 100, 200]
    TEST_ASSERT(dlist_size(&list) == 3);

    DListNode *found = dlist_find(&list, 100);
    TEST_ASSERT(found && found->value == 100);
    TEST_ASSERT(dlist_remove_node(&list, found) == DS_SUCCESS);
    TEST_ASSERT(dlist_size(&list) == 2);

    int val = 0;
    TEST_ASSERT(dlist_pop_front(&list, &val) == DS_SUCCESS && val == 50);
    TEST_ASSERT(dlist_pop_back(&list, &val) == DS_SUCCESS && val == 200);
    TEST_ASSERT(dlist_is_empty(&list));

    dlist_free(&list);
}

/* --- Stack Tests --- */
void test_stack() {
    Stack stack;
    TEST_ASSERT(stack_init(&stack, sizeof(int), 4) == DS_SUCCESS);

    for (int i = 1; i <= 5; i++) {
        TEST_ASSERT(stack_push(&stack, &i) == DS_SUCCESS);
    }
    TEST_ASSERT(stack_size(&stack) == 5);

    int *top = (int *)stack_peek(&stack);
    TEST_ASSERT(top && *top == 5);

    int val = 0;
    TEST_ASSERT(stack_pop(&stack, &val) == DS_SUCCESS && val == 5);
    TEST_ASSERT(stack_pop(&stack, &val) == DS_SUCCESS && val == 4);
    TEST_ASSERT(stack_size(&stack) == 3);

    stack_free(&stack);
    TEST_ASSERT(stack_is_empty(&stack));
}

/* --- Queue Tests --- */
void test_queue() {
    Queue q;
    TEST_ASSERT(queue_init(&q, sizeof(int), 4) == DS_SUCCESS);

    for (int i = 1; i <= 3; i++) {
        TEST_ASSERT(queue_enqueue(&q, &i) == DS_SUCCESS);
    }

    int val = 0;
    TEST_ASSERT(queue_dequeue(&q, &val) == DS_SUCCESS && val == 1);
    TEST_ASSERT(queue_dequeue(&q, &val) == DS_SUCCESS && val == 2);

    for (int i = 4; i <= 10; i++) {
        TEST_ASSERT(queue_enqueue(&q, &i) == DS_SUCCESS);
    }

    int expected = 3;
    while (!queue_is_empty(&q)) {
        TEST_ASSERT(queue_dequeue(&q, &val) == DS_SUCCESS);
        TEST_ASSERT(val == expected);
        expected++;
    }
    TEST_ASSERT(expected == 11);

    queue_free(&q);
}

/* --- BST Tests --- */
static int g_inorder_arr[20];
static size_t g_inorder_idx = 0;
static void collect_inorder(int key, void *user_data) {
    (void)user_data;
    g_inorder_arr[g_inorder_idx++] = key;
}

void test_bst() {
    BinarySearchTree tree;
    TEST_ASSERT(bst_init(&tree) == DS_SUCCESS);

    // Insert keys: 50, 30, 70, 20, 40, 60, 80
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT(bst_insert(&tree, keys[i]) == DS_SUCCESS);
    }
    TEST_ASSERT(bst_insert(&tree, 50) == DS_ERR_DUPLICATE);
    TEST_ASSERT(bst_size(&tree) == 7);

    TEST_ASSERT(bst_search(&tree, 40) == true);
    TEST_ASSERT(bst_search(&tree, 99) == false);

    int min_val = 0, max_val = 0;
    TEST_ASSERT(bst_min(&tree, &min_val) == DS_SUCCESS && min_val == 20);
    TEST_ASSERT(bst_max(&tree, &max_val) == DS_SUCCESS && max_val == 80);

    g_inorder_idx = 0;
    bst_inorder(&tree, collect_inorder, NULL);
    TEST_ASSERT(g_inorder_idx == 7);
    int expected_sorted[] = {20, 30, 40, 50, 60, 70, 80};
    for (size_t i = 0; i < 7; i++) {
        TEST_ASSERT(g_inorder_arr[i] == expected_sorted[i]);
    }

    // Delete leaf (20)
    TEST_ASSERT(bst_delete(&tree, 20) == DS_SUCCESS);
    TEST_ASSERT(!bst_search(&tree, 20));

    // Delete node with 2 children (50)
    TEST_ASSERT(bst_delete(&tree, 50) == DS_SUCCESS);
    TEST_ASSERT(!bst_search(&tree, 50));
    TEST_ASSERT(bst_size(&tree) == 5);

    bst_free(&tree);
    TEST_ASSERT(bst_is_empty(&tree));
}

int main(void) {
    printf("========================================\n");
    printf("   Running C Data Structures Test Suite \n");
    printf("========================================\n");

    RUN_TEST(test_vector);
    RUN_TEST(test_slist);
    RUN_TEST(test_dlist);
    RUN_TEST(test_stack);
    RUN_TEST(test_queue);
    RUN_TEST(test_bst);

    printf("\nAll %d C tests passed successfully!\n", g_tests_passed);
    return 0;
}
