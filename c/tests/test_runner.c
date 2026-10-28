#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "bst.h"
#include "heap.h"
#include "hash_table.h"
#include "trie.h"
#include "dsu.h"
#include "avl_tree.h"

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

/* --- Binary Heap Tests --- */
static int int_min_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static int int_max_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ib > ia) - (ib < ia);
}

void test_heap() {
    BinaryHeap min_heap;
    TEST_ASSERT(heap_init(&min_heap, sizeof(int), 4, int_min_cmp) == DS_SUCCESS);
    TEST_ASSERT(heap_is_empty(&min_heap));

    int values[] = {45, 20, 14, 12, 31, 7, 11, 13, 7};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        TEST_ASSERT(heap_push(&min_heap, &values[i]) == DS_SUCCESS);
    }
    TEST_ASSERT(heap_size(&min_heap) == (size_t)n);

    int top = 0;
    TEST_ASSERT(heap_peek(&min_heap, &top) == DS_SUCCESS);
    TEST_ASSERT(top == 7);

    // Extract all elements and verify ascending order
    int prev = -1;
    for (int i = 0; i < n; i++) {
        int curr = 0;
        TEST_ASSERT(heap_pop(&min_heap, &curr) == DS_SUCCESS);
        if (i > 0) {
            TEST_ASSERT(curr >= prev);
        }
        prev = curr;
    }
    TEST_ASSERT(heap_is_empty(&min_heap));
    heap_destroy(&min_heap);

    // Test Max-Heap
    BinaryHeap max_heap;
    TEST_ASSERT(heap_init(&max_heap, sizeof(int), 4, int_max_cmp) == DS_SUCCESS);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT(heap_push(&max_heap, &values[i]) == DS_SUCCESS);
    }
    TEST_ASSERT(heap_peek(&max_heap, &top) == DS_SUCCESS);
    TEST_ASSERT(top == 45);

    prev = 1000;
    for (int i = 0; i < n; i++) {
        int curr = 0;
        TEST_ASSERT(heap_pop(&max_heap, &curr) == DS_SUCCESS);
        TEST_ASSERT(curr <= prev);
        prev = curr;
    }
    heap_destroy(&max_heap);
}

/* --- Hash Table Tests --- */
void test_hash_table() {
    HashTable ht;
    TEST_ASSERT(ht_init(&ht, 4) == DS_SUCCESS);
    TEST_ASSERT(ht_is_empty(&ht));

    int val1 = 10, val2 = 20, val3 = 30, val4 = 40, val5 = 50;
    TEST_ASSERT(ht_insert(&ht, "alpha", &val1) == DS_SUCCESS);
    TEST_ASSERT(ht_insert(&ht, "beta", &val2) == DS_SUCCESS);
    TEST_ASSERT(ht_insert(&ht, "gamma", &val3) == DS_SUCCESS);
    TEST_ASSERT(ht_insert(&ht, "delta", &val4) == DS_SUCCESS);
    TEST_ASSERT(ht_insert(&ht, "epsilon", &val5) == DS_SUCCESS);

    TEST_ASSERT(ht_size(&ht) == 5);
    TEST_ASSERT(ht_contains(&ht, "alpha"));
    TEST_ASSERT(ht_contains(&ht, "gamma"));
    TEST_ASSERT(!ht_contains(&ht, "zeta"));

    void *ret = NULL;
    TEST_ASSERT(ht_get(&ht, "alpha", &ret) == DS_SUCCESS && *(int *)ret == 10);
    TEST_ASSERT(ht_get(&ht, "epsilon", &ret) == DS_SUCCESS && *(int *)ret == 50);

    // Update existing key
    int updated_val = 999;
    TEST_ASSERT(ht_insert(&ht, "beta", &updated_val) == DS_SUCCESS);
    TEST_ASSERT(ht_get(&ht, "beta", &ret) == DS_SUCCESS && *(int *)ret == 999);
    TEST_ASSERT(ht_size(&ht) == 5);

    // Remove key
    TEST_ASSERT(ht_remove(&ht, "gamma", &ret) == DS_SUCCESS && *(int *)ret == 30);
    TEST_ASSERT(!ht_contains(&ht, "gamma"));
    TEST_ASSERT(ht_size(&ht) == 4);

    ht_destroy(&ht, NULL);
    TEST_ASSERT(ht_is_empty(&ht));
}

/* --- Trie Tests --- */
void test_trie() {
    Trie trie;
    TEST_ASSERT(trie_init(&trie) == DS_SUCCESS);
    TEST_ASSERT(trie_is_empty(&trie));

    TEST_ASSERT(trie_insert(&trie, "apple") == DS_SUCCESS);
    TEST_ASSERT(trie_insert(&trie, "app") == DS_SUCCESS);
    TEST_ASSERT(trie_insert(&trie, "application") == DS_SUCCESS);
    TEST_ASSERT(trie_insert(&trie, "bat") == DS_SUCCESS);
    TEST_ASSERT(trie_insert(&trie, "batch") == DS_SUCCESS);

    TEST_ASSERT(trie_size(&trie) == 5);
    TEST_ASSERT(trie_search(&trie, "apple"));
    TEST_ASSERT(trie_search(&trie, "app"));
    TEST_ASSERT(trie_search(&trie, "bat"));
    TEST_ASSERT(!trie_search(&trie, "appl"));
    TEST_ASSERT(!trie_search(&trie, "cat"));

    // Prefix search
    TEST_ASSERT(trie_starts_with(&trie, "app"));
    TEST_ASSERT(trie_starts_with(&trie, "applic"));
    TEST_ASSERT(trie_starts_with(&trie, "bat"));
    TEST_ASSERT(!trie_starts_with(&trie, "xyz"));

    // Removal
    TEST_ASSERT(trie_remove(&trie, "app") == DS_SUCCESS);
    TEST_ASSERT(!trie_search(&trie, "app"));
    TEST_ASSERT(trie_search(&trie, "apple")); // child still exists
    TEST_ASSERT(trie_size(&trie) == 4);

    trie_destroy(&trie);
    TEST_ASSERT(trie_is_empty(&trie));
}

/* --- DSU Tests --- */
void test_dsu() {
    DSU dsu;
    TEST_ASSERT(dsu_init(&dsu, 6) == DS_SUCCESS);
    TEST_ASSERT(dsu_set_count(&dsu) == 6);

    for (size_t i = 0; i < 6; i++) {
        for (size_t j = i + 1; j < 6; j++) {
            TEST_ASSERT(!dsu_connected(&dsu, i, j));
        }
    }

    bool merged = false;
    TEST_ASSERT(dsu_union(&dsu, 0, 1, &merged) == DS_SUCCESS && merged);
    TEST_ASSERT(dsu_union(&dsu, 1, 2, &merged) == DS_SUCCESS && merged);
    TEST_ASSERT(dsu_connected(&dsu, 0, 2));
    TEST_ASSERT(!dsu_connected(&dsu, 0, 3));
    TEST_ASSERT(dsu_set_count(&dsu) == 4);

    // Union already connected elements
    TEST_ASSERT(dsu_union(&dsu, 0, 2, &merged) == DS_SUCCESS && !merged);
    TEST_ASSERT(dsu_set_count(&dsu) == 4);

    TEST_ASSERT(dsu_union(&dsu, 3, 4, &merged) == DS_SUCCESS && merged);
    TEST_ASSERT(dsu_union(&dsu, 4, 5, &merged) == DS_SUCCESS && merged);
    TEST_ASSERT(dsu_set_count(&dsu) == 2);

    // Merge both components
    TEST_ASSERT(dsu_union(&dsu, 2, 3, &merged) == DS_SUCCESS && merged);
    TEST_ASSERT(dsu_set_count(&dsu) == 1);
    TEST_ASSERT(dsu_connected(&dsu, 0, 5));

    dsu_destroy(&dsu);
}

/* --- AVL Tree Tests --- */
static int g_avl_arr[32];
static size_t g_avl_idx = 0;
static void avl_collect_inorder(int key) {
    if (g_avl_idx < 32) {
        g_avl_arr[g_avl_idx++] = key;
    }
}

void test_avl() {
    AVLTree tree;
    TEST_ASSERT(avl_init(&tree) == DS_SUCCESS);
    TEST_ASSERT(avl_is_empty(&tree));

    // Insert keys sequentially to trigger rotations
    // Sequential insertions in normal BST would create skewed line of height 7
    int keys[] = {10, 20, 30, 40, 50, 25, 5};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT(avl_insert(&tree, keys[i]) == DS_SUCCESS);
    }
    TEST_ASSERT(avl_size(&tree) == 7);
    TEST_ASSERT(avl_height(&tree) <= 4); // AVL guaranteed logarithmic balance!

    TEST_ASSERT(avl_search(&tree, 25));
    TEST_ASSERT(avl_search(&tree, 10));
    TEST_ASSERT(!avl_search(&tree, 999));

    // Verify in-order traversal is sorted
    g_avl_idx = 0;
    avl_inorder(&tree, avl_collect_inorder);
    TEST_ASSERT(g_avl_idx == 7);
    for (size_t i = 1; i < g_avl_idx; i++) {
        TEST_ASSERT(g_avl_arr[i] > g_avl_arr[i - 1]);
    }

    // Delete node and verify self-balancing remains intact
    TEST_ASSERT(avl_delete(&tree, 30) == DS_SUCCESS);
    TEST_ASSERT(!avl_search(&tree, 30));
    TEST_ASSERT(avl_size(&tree) == 6);
    TEST_ASSERT(avl_height(&tree) <= 4);

    avl_destroy(&tree);
    TEST_ASSERT(avl_is_empty(&tree));
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
    RUN_TEST(test_heap);
    RUN_TEST(test_hash_table);
    RUN_TEST(test_trie);
    RUN_TEST(test_dsu);
    RUN_TEST(test_avl);

    printf("\nAll %d C tests passed successfully!\n", g_tests_passed);
    return 0;
}
