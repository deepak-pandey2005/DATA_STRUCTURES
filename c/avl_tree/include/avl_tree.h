/**
 * @file avl_tree.h
 * @brief Self-Balancing AVL Tree in C11.
 */

#ifndef DS_AVL_TREE_H
#define DS_AVL_TREE_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AVLNode {
    int key;                    /**< Comparable integer key */
    int height;                 /**< Height of this subtree */
    struct AVLNode *left;       /**< Left child pointer */
    struct AVLNode *right;      /**< Right child pointer */
} AVLNode;

typedef struct {
    AVLNode *root;              /**< Root node of the AVL tree */
    size_t size;                /**< Total number of nodes */
} AVLTree;

/**
 * @brief Initializes an empty AVL tree.
 */
ds_status_t avl_init(AVLTree *tree);

/**
 * @brief Frees all nodes in the AVL tree.
 */
void avl_destroy(AVLTree *tree);

/**
 * @brief Inserts a key into the AVL tree, balancing the tree via rotations.
 */
ds_status_t avl_insert(AVLTree *tree, int key);

/**
 * @brief Deletes a key from the AVL tree, rebalancing the tree via rotations.
 */
ds_status_t avl_delete(AVLTree *tree, int key);

/**
 * @brief Searches for key in the AVL tree.
 */
bool avl_search(const AVLTree *tree, int key);

/**
 * @brief Returns height of the root node (0 if empty).
 */
int avl_height(const AVLTree *tree);

/**
 * @brief Returns total number of nodes in the AVL tree.
 */
size_t avl_size(const AVLTree *tree);

/**
 * @brief Checks if the AVL tree is empty.
 */
bool avl_is_empty(const AVLTree *tree);

/**
 * @brief In-order traversal executing callback on each node key.
 */
void avl_inorder(const AVLTree *tree, void (*callback)(int key));

#ifdef __cplusplus
}
#endif

#endif // DS_AVL_TREE_H

