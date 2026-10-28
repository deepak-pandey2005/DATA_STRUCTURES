/**
 * @file avl_tree.c
 * @brief Implementation of AVL Tree in C11.
 */

#include "avl_tree.h"
#include <stdlib.h>

static inline int max(int a, int b) {
    return (a > b) ? a : b;
}

static inline int node_height(const AVLNode *node) {
    return node ? node->height : 0;
}

static inline int balance_factor(const AVLNode *node) {
    return node ? node_height(node->left) - node_height(node->right) : 0;
}

static inline void update_height(AVLNode *node) {
    if (node) {
        node->height = 1 + max(node_height(node->left), node_height(node->right));
    }
}

// Right rotation around y
static AVLNode *rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);
    return x;
}

// Left rotation around x
static AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);
    return y;
}

static AVLNode *avl_node_create(int key) {
    AVLNode *node = malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void avl_node_free(AVLNode *node) {
    if (!node) return;
    avl_node_free(node->left);
    avl_node_free(node->right);
    free(node);
}

static AVLNode *avl_insert_helper(AVLNode *node, int key, bool *inserted, ds_status_t *status) {
    if (!node) {
        AVLNode *new_node = avl_node_create(key);
        if (!new_node) {
            *status = DS_ERR_OUT_OF_MEMORY;
            return NULL;
        }
        *inserted = true;
        *status = DS_SUCCESS;
        return new_node;
    }

    if (key < node->key) {
        node->left = avl_insert_helper(node->left, key, inserted, status);
    } else if (key > node->key) {
        node->right = avl_insert_helper(node->right, key, inserted, status);
    } else {
        *inserted = false;
        *status = DS_ERR_DUPLICATE;
        return node;
    }

    if (!*inserted) return node;

    update_height(node);
    int bf = balance_factor(node);

    // Left-Left
    if (bf > 1 && key < node->left->key) {
        return rotate_right(node);
    }
    // Right-Right
    if (bf < -1 && key > node->right->key) {
        return rotate_left(node);
    }
    // Left-Right
    if (bf > 1 && key > node->left->key) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    // Right-Left
    if (bf < -1 && key < node->right->key) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

static AVLNode *min_node(AVLNode *node) {
    AVLNode *curr = node;
    while (curr && curr->left) {
        curr = curr->left;
    }
    return curr;
}

static AVLNode *avl_delete_helper(AVLNode *node, int key, bool *deleted) {
    if (!node) return NULL;

    if (key < node->key) {
        node->left = avl_delete_helper(node->left, key, deleted);
    } else if (key > node->key) {
        node->right = avl_delete_helper(node->right, key, deleted);
    } else {
        *deleted = true;
        // Node with only one child or no child
        if (!node->left || !node->right) {
            AVLNode *temp = node->left ? node->left : node->right;
            if (!temp) {
                // No child
                free(node);
                return NULL;
            } else {
                // One child
                *node = *temp;
                free(temp);
            }
        } else {
            // Node with two children: get inorder successor
            AVLNode *succ = min_node(node->right);
            node->key = succ->key;
            node->right = avl_delete_helper(node->right, succ->key, deleted);
        }
    }

    if (!node) return NULL;

    update_height(node);
    int bf = balance_factor(node);

    // Left-Left
    if (bf > 1 && balance_factor(node->left) >= 0) {
        return rotate_right(node);
    }
    // Left-Right
    if (bf > 1 && balance_factor(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    // Right-Right
    if (bf < -1 && balance_factor(node->right) <= 0) {
        return rotate_left(node);
    }
    // Right-Left
    if (bf < -1 && balance_factor(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

ds_status_t avl_init(AVLTree *tree) {
    if (!tree) return DS_ERR_NULL_PTR;
    tree->root = NULL;
    tree->size = 0;
    return DS_SUCCESS;
}

void avl_destroy(AVLTree *tree) {
    if (!tree) return;
    avl_node_free(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

ds_status_t avl_insert(AVLTree *tree, int key) {
    if (!tree) return DS_ERR_NULL_PTR;
    bool inserted = false;
    ds_status_t status = DS_SUCCESS;
    tree->root = avl_insert_helper(tree->root, key, &inserted, &status);
    if (inserted) {
        tree->size++;
    }
    return status;
}

ds_status_t avl_delete(AVLTree *tree, int key) {
    if (!tree) return DS_ERR_NULL_PTR;
    bool deleted = false;
    tree->root = avl_delete_helper(tree->root, key, &deleted);
    if (deleted) {
        tree->size--;
        return DS_SUCCESS;
    }
    return DS_ERR_NOT_FOUND;
}

bool avl_search(const AVLTree *tree, int key) {
    if (!tree) return false;
    const AVLNode *curr = tree->root;
    while (curr) {
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            return true;
        }
    }
    return false;
}

int avl_height(const AVLTree *tree) {
    return (tree && tree->root) ? tree->root->height : 0;
}

size_t avl_size(const AVLTree *tree) {
    return tree ? tree->size : 0;
}

bool avl_is_empty(const AVLTree *tree) {
    return !tree || tree->size == 0;
}

static void inorder_helper(const AVLNode *node, void (*callback)(int key)) {
    if (!node) return;
    inorder_helper(node->left, callback);
    callback(node->key);
    inorder_helper(node->right, callback);
}

void avl_inorder(const AVLTree *tree, void (*callback)(int key)) {
    if (!tree || !callback) return;
    inorder_helper(tree->root, callback);
}

