/**
 * @file main.c
 * @brief Standalone runnable demonstration for Binary Search Tree in C11.
 */

#include <stdio.h>
#include "bst.h"

static void print_node(int key, void *user_data) {
    (void)user_data;
    printf("%d ", key);
}

int main(void) {
    printf("=== [06] Binary Search Tree Standalone Demo (C11) ===\n\n");

    BinarySearchTree tree;
    bst_init(&tree);

    int sample_keys[] = {50, 30, 70, 20, 40, 60, 80};
    printf("Inserting keys: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", sample_keys[i]);
        bst_insert(&tree, sample_keys[i]);
    }
    printf("\nTree size: %zu\n", bst_size(&tree));

    printf("In-order sorted traversal: ");
    bst_inorder(&tree, print_node, NULL);
    printf("\n");

    int search_key = 40;
    printf("Searching for %d: %s\n", search_key, bst_search(&tree, search_key) ? "FOUND" : "NOT FOUND");

    printf("Deleting root node 50 (two children)...\n");
    bst_delete(&tree, 50);

    printf("In-order after deletion: ");
    bst_inorder(&tree, print_node, NULL);
    printf("\nNew size: %zu\n", bst_size(&tree));

    bst_free(&tree);
    printf("\nTree cleaned up successfully.\n");
    return 0;
}
