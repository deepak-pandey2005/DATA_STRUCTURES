#ifndef DS_BST_H
#define DS_BST_H

#include "common.h"

typedef struct BSTNode {
    int key;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
    size_t size;
} BinarySearchTree;

typedef void (*bst_visitor_fn)(int key, void *user_data);

ds_status_t bst_init(BinarySearchTree *tree);
ds_status_t bst_insert(BinarySearchTree *tree, int key);
ds_status_t bst_delete(BinarySearchTree *tree, int key);
bool bst_search(const BinarySearchTree *tree, int key);
ds_status_t bst_min(const BinarySearchTree *tree, int *out_min);
ds_status_t bst_max(const BinarySearchTree *tree, int *out_max);
size_t bst_height(const BinarySearchTree *tree);
size_t bst_size(const BinarySearchTree *tree);
bool bst_is_empty(const BinarySearchTree *tree);
void bst_inorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data);
void bst_preorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data);
void bst_postorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data);
void bst_clear(BinarySearchTree *tree);
void bst_free(BinarySearchTree *tree);

#endif /* DS_BST_H */
