#include "BinarySearchTree.hpp"
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <queue>
#include <string>

namespace ds {

// Node implementations
template <typename T>
BinarySearchTree<T>::Node::Node(const T &k) : key(k), left(nullptr), right(nullptr) {}

template <typename T>
BinarySearchTree<T>::Node::Node(T &&k) : key(std::move(k)), left(nullptr), right(nullptr) {}

// BST implementations
template <typename T>
BinarySearchTree<T>::BinarySearchTree() : root_(nullptr), size_(0) {}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    clear();
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree &other)
    : root_(clone(other.root_)), size_(other.size_) {}

template <typename T>
BinarySearchTree<T> &BinarySearchTree<T>::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        BinarySearchTree temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree &&other) noexcept
    : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename T>
BinarySearchTree<T> &BinarySearchTree<T>::operator=(BinarySearchTree &&other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void BinarySearchTree<T>::swap(BinarySearchTree &other) noexcept {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::clone(Node *node) {
    if (!node) return nullptr;
    Node *new_node = new Node(node->key);
    new_node->left = clone(node->left);
    new_node->right = clone(node->right);
    return new_node;
}

template <typename T>
void BinarySearchTree<T>::destroy(Node *node) noexcept {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::insert_node(Node *node, const T &key, bool &inserted) {
    if (!node) {
        inserted = true;
        return new Node(key);
    }
    if (key < node->key) {
        node->left = insert_node(node->left, key, inserted);
    } else if (node->key < key) {
        node->right = insert_node(node->right, key, inserted);
    } else {
        inserted = false; // duplicate
    }
    return node;
}

template <typename T>
bool BinarySearchTree<T>::insert(const T &key) {
    bool inserted = false;
    root_ = insert_node(root_, key, inserted);
    if (inserted) ++size_;
    return inserted;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::find_min(Node *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::remove_node(Node *node, const T &key, bool &removed) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = remove_node(node->left, key, removed);
    } else if (node->key < key) {
        node->right = remove_node(node->right, key, removed);
    } else {
        removed = true;
        if (!node->left) {
            Node *right_child = node->right;
            delete node;
            return right_child;
        } else if (!node->right) {
            Node *left_child = node->left;
            delete node;
            return left_child;
        } else {
            Node *succ = find_min(node->right);
            node->key = succ->key;
            node->right = remove_node(node->right, succ->key, removed);
        }
    }
    return node;
}

template <typename T>
bool BinarySearchTree<T>::remove(const T &key) {
    bool removed = false;
    root_ = remove_node(root_, key, removed);
    if (removed) --size_;
    return removed;
}

template <typename T>
bool BinarySearchTree<T>::contains(const T &key) const {
    Node *curr = root_;
    while (curr) {
        if (key < curr->key) {
            curr = curr->left;
        } else if (curr->key < key) {
            curr = curr->right;
        } else {
            return true;
        }
    }
    return false;
}

template <typename T>
const T &BinarySearchTree<T>::min() const {
    if (empty()) throw std::underflow_error("Tree is empty");
    return find_min(root_)->key;
}

template <typename T>
const T &BinarySearchTree<T>::max() const {
    if (empty()) throw std::underflow_error("Tree is empty");
    Node *curr = root_;
    while (curr->right) curr = curr->right;
    return curr->key;
}

template <typename T>
std::size_t BinarySearchTree<T>::height_node(const Node *node) {
    if (!node) return 0;
    return 1 + std::max(height_node(node->left), height_node(node->right));
}

template <typename T>
std::size_t BinarySearchTree<T>::height() const noexcept {
    return height_node(root_);
}

template <typename T>
void BinarySearchTree<T>::inorder_node(Node *node, const std::function<void(const T &)> &visitor) {
    if (!node) return;
    inorder_node(node->left, visitor);
    visitor(node->key);
    inorder_node(node->right, visitor);
}

template <typename T>
void BinarySearchTree<T>::inorder(const std::function<void(const T &)> &visitor) const {
    inorder_node(root_, visitor);
}

template <typename T>
void BinarySearchTree<T>::preorder_node(Node *node, const std::function<void(const T &)> &visitor) {
    if (!node) return;
    visitor(node->key);
    preorder_node(node->left, visitor);
    preorder_node(node->right, visitor);
}

template <typename T>
void BinarySearchTree<T>::preorder(const std::function<void(const T &)> &visitor) const {
    preorder_node(root_, visitor);
}

template <typename T>
void BinarySearchTree<T>::postorder_node(Node *node, const std::function<void(const T &)> &visitor) {
    if (!node) return;
    postorder_node(node->left, visitor);
    postorder_node(node->right, visitor);
    visitor(node->key);
}

template <typename T>
void BinarySearchTree<T>::postorder(const std::function<void(const T &)> &visitor) const {
    postorder_node(root_, visitor);
}

template <typename T>
void BinarySearchTree<T>::level_order(const std::function<void(const T &)> &visitor) const {
    if (!root_) return;
    std::queue<Node *> q;
    q.push(root_);
    while (!q.empty()) {
        Node *curr = q.front();
        q.pop();
        visitor(curr->key);
        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}

template <typename T>
std::vector<T> BinarySearchTree<T>::inorder_keys() const {
    std::vector<T> res;
    res.reserve(size_);
    inorder([&res](const T &k) { res.push_back(k); });
    return res;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::preorder_keys() const {
    std::vector<T> res;
    res.reserve(size_);
    preorder([&res](const T &k) { res.push_back(k); });
    return res;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::postorder_keys() const {
    std::vector<T> res;
    res.reserve(size_);
    postorder([&res](const T &k) { res.push_back(k); });
    return res;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::level_order_keys() const {
    std::vector<T> res;
    res.reserve(size_);
    level_order([&res](const T &k) { res.push_back(k); });
    return res;
}

template <typename T>
void BinarySearchTree<T>::clear() noexcept {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
}

template <typename T>
std::size_t BinarySearchTree<T>::size() const noexcept { return size_; }

template <typename T>
bool BinarySearchTree<T>::empty() const noexcept { return size_ == 0; }

// Explicit template instantiations
template class BinarySearchTree<int>;
template class BinarySearchTree<double>;
template class BinarySearchTree<char>;
template class BinarySearchTree<std::string>;

} // namespace ds
