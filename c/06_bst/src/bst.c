#include "bst.h"
#include <stdlib.h>

ds_status_t bst_init(BinarySearchTree *tree) {
    if (!tree) return DS_ERR_NULL_PTR;
    tree->root = NULL;
    tree->size = 0;
    return DS_SUCCESS;
}

static BSTNode *bst_create_node(int key) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static BSTNode *bst_insert_node(BSTNode *root, int key, bool *inserted) {
    if (!root) {
        *inserted = true;
        return bst_create_node(key);
    }
    if (key < root->key) {
        root->left = bst_insert_node(root->left, key, inserted);
    } else if (key > root->key) {
        root->right = bst_insert_node(root->right, key, inserted);
    } else {
        *inserted = false; // duplicate
    }
    return root;
}

ds_status_t bst_insert(BinarySearchTree *tree, int key) {
    if (!tree) return DS_ERR_NULL_PTR;

    bool inserted = false;
    BSTNode *new_root = bst_insert_node(tree->root, key, &inserted);
    if (!inserted) {
        return DS_ERR_DUPLICATE;
    }
    if (!new_root && tree->root == NULL) {
        return DS_ERR_OUT_OF_MEMORY;
    }

    tree->root = new_root;
    tree->size++;
    return DS_SUCCESS;
}

bool bst_search(const BinarySearchTree *tree, int key) {
    if (!tree) return false;
    BSTNode *curr = tree->root;
    while (curr) {
        if (key == curr->key) return true;
        if (key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
    return false;
}

static BSTNode *bst_find_min_node(BSTNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

static BSTNode *bst_delete_node(BSTNode *root, int key, bool *deleted) {
    if (!root) return NULL;

    if (key < root->key) {
        root->left = bst_delete_node(root->left, key, deleted);
    } else if (key > root->key) {
        root->right = bst_delete_node(root->right, key, deleted);
    } else {
        *deleted = true;
        // Case 1: No child or 1 child
        if (!root->left) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }

        // Case 2: 2 children - get inorder successor (min in right subtree)
        BSTNode *succ = bst_find_min_node(root->right);
        root->key = succ->key;
        root->right = bst_delete_node(root->right, succ->key, deleted);
    }
    return root;
}

ds_status_t bst_delete(BinarySearchTree *tree, int key) {
    if (!tree) return DS_ERR_NULL_PTR;
    if (!tree->root) return DS_ERR_EMPTY;

    bool deleted = false;
    tree->root = bst_delete_node(tree->root, key, &deleted);
    if (deleted) {
        tree->size--;
        return DS_SUCCESS;
    }
    return DS_ERR_NOT_FOUND;
}

ds_status_t bst_min(const BinarySearchTree *tree, int *out_min) {
    if (!tree || !out_min) return DS_ERR_NULL_PTR;
    if (!tree->root) return DS_ERR_EMPTY;

    BSTNode *min_node = bst_find_min_node(tree->root);
    *out_min = min_node->key;
    return DS_SUCCESS;
}

ds_status_t bst_max(const BinarySearchTree *tree, int *out_max) {
    if (!tree || !out_max) return DS_ERR_NULL_PTR;
    if (!tree->root) return DS_ERR_EMPTY;

    BSTNode *curr = tree->root;
    while (curr->right) {
        curr = curr->right;
    }
    *out_max = curr->key;
    return DS_SUCCESS;
}

static size_t bst_node_height(const BSTNode *node) {
    if (!node) return 0;
    size_t lh = bst_node_height(node->left);
    size_t rh = bst_node_height(node->right);
    return 1 + (lh > rh ? lh : rh);
}

size_t bst_height(const BinarySearchTree *tree) {
    if (!tree) return 0;
    return bst_node_height(tree->root);
}

size_t bst_size(const BinarySearchTree *tree) {
    return tree ? tree->size : 0;
}

bool bst_is_empty(const BinarySearchTree *tree) {
    return (!tree || tree->size == 0);
}

static void bst_inorder_node(const BSTNode *node, bst_visitor_fn visitor, void *user_data) {
    if (!node) return;
    bst_inorder_node(node->left, visitor, user_data);
    visitor(node->key, user_data);
    bst_inorder_node(node->right, visitor, user_data);
}

void bst_inorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data) {
    if (!tree || !visitor) return;
    bst_inorder_node(tree->root, visitor, user_data);
}

static void bst_preorder_node(const BSTNode *node, bst_visitor_fn visitor, void *user_data) {
    if (!node) return;
    visitor(node->key, user_data);
    bst_preorder_node(node->left, visitor, user_data);
    bst_preorder_node(node->right, visitor, user_data);
}

void bst_preorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data) {
    if (!tree || !visitor) return;
    bst_preorder_node(tree->root, visitor, user_data);
}

static void bst_postorder_node(const BSTNode *node, bst_visitor_fn visitor, void *user_data) {
    if (!node) return;
    bst_postorder_node(node->left, visitor, user_data);
    bst_postorder_node(node->right, visitor, user_data);
    visitor(node->key, user_data);
}

void bst_postorder(const BinarySearchTree *tree, bst_visitor_fn visitor, void *user_data) {
    if (!tree || !visitor) return;
    bst_postorder_node(tree->root, visitor, user_data);
}

static void bst_free_node(BSTNode *node) {
    if (!node) return;
    bst_free_node(node->left);
    bst_free_node(node->right);
    free(node);
}

void bst_clear(BinarySearchTree *tree) {
    if (!tree) return;
    bst_free_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

void bst_free(BinarySearchTree *tree) {
    bst_clear(tree);
}
