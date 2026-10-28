/**
 * @file AVLTree.hpp
 * @brief Modern C++20 Self-Balancing AVL Tree.
 */

#ifndef DS_AVL_TREE_HPP
#define DS_AVL_TREE_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstddef>

namespace ds {

template <typename T>
class AVLTree {
public:
    AVLTree() : root_(nullptr), size_(0) {}
    ~AVLTree() = default;

    AVLTree(const AVLTree &other) : root_(nullptr), size_(0) {
        root_ = copy_node(other.root_.get());
        size_ = other.size_;
    }

    AVLTree &operator=(const AVLTree &other) {
        if (this != &other) {
            clear();
            root_ = copy_node(other.root_.get());
            size_ = other.size_;
        }
        return *this;
    }

    AVLTree(AVLTree &&) noexcept = default;
    AVLTree &operator=(AVLTree &&) noexcept = default;

    /**
     * @brief Inserts a value into the AVL tree in O(log n).
     */
    bool insert(const T &val) {
        bool inserted = false;
        root_ = insert_helper(std::move(root_), val, inserted);
        if (inserted) size_++;
        return inserted;
    }

    /**
     * @brief Removes a value from the AVL tree in O(log n).
     */
    bool remove(const T &val) {
        bool deleted = false;
        root_ = remove_helper(std::move(root_), val, deleted);
        if (deleted) size_--;
        return deleted;
    }

    /**
     * @brief Searches for a value in O(log n).
     */
    [[nodiscard]] bool contains(const T &val) const {
        const Node *curr = root_.get();
        while (curr) {
            if (val < curr->data) {
                curr = curr->left.get();
            } else if (curr->data < val) {
                curr = curr->right.get();
            } else {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] int height() const noexcept {
        return get_height(root_.get());
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    void clear() noexcept {
        root_.reset();
        size_ = 0;
    }

    [[nodiscard]] std::vector<T> inorder() const {
        std::vector<T> result;
        inorder_helper(root_.get(), result);
        return result;
    }

private:
    struct Node {
        T data;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(const T &val) : data(val), height(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root_;
    std::size_t size_;

    static int get_height(const Node *node) noexcept {
        return node ? node->height : 0;
    }

    static int get_balance(const Node *node) noexcept {
        return node ? get_height(node->left.get()) - get_height(node->right.get()) : 0;
    }

    static void update_height(Node *node) noexcept {
        if (node) {
            node->height = 1 + std::max(get_height(node->left.get()), get_height(node->right.get()));
        }
    }

    static std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> y) {
        std::unique_ptr<Node> x = std::move(y->left);
        y->left = std::move(x->right);
        update_height(y.get());
        x->right = std::move(y);
        update_height(x.get());
        return x;
    }

    static std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> x) {
        std::unique_ptr<Node> y = std::move(x->right);
        x->right = std::move(y->left);
        update_height(x.get());
        y->left = std::move(x);
        update_height(y.get());
        return y;
    }

    std::unique_ptr<Node> insert_helper(std::unique_ptr<Node> node, const T &val, bool &inserted) {
        if (!node) {
            inserted = true;
            return std::make_unique<Node>(val);
        }

        if (val < node->data) {
            node->left = insert_helper(std::move(node->left), val, inserted);
        } else if (node->data < val) {
            node->right = insert_helper(std::move(node->right), val, inserted);
        } else {
            inserted = false;
            return node;
        }

        if (!inserted) return node;

        update_height(node.get());
        int bf = get_balance(node.get());

        // LL
        if (bf > 1 && val < node->left->data) {
            return rotate_right(std::move(node));
        }
        // RR
        if (bf < -1 && node->right->data < val) {
            return rotate_left(std::move(node));
        }
        // LR
        if (bf > 1 && node->left->data < val) {
            node->left = rotate_left(std::move(node->left));
            return rotate_right(std::move(node));
        }
        // RL
        if (bf < -1 && val < node->right->data) {
            node->right = rotate_right(std::move(node->right));
            return rotate_left(std::move(node));
        }

        return node;
    }

    static Node *find_min(Node *node) {
        Node *curr = node;
        while (curr && curr->left) {
            curr = curr->left.get();
        }
        return curr;
    }

    std::unique_ptr<Node> remove_helper(std::unique_ptr<Node> node, const T &val, bool &deleted) {
        if (!node) return nullptr;

        if (val < node->data) {
            node->left = remove_helper(std::move(node->left), val, deleted);
        } else if (node->data < val) {
            node->right = remove_helper(std::move(node->right), val, deleted);
        } else {
            deleted = true;
            if (!node->left || !node->right) {
                return std::move(node->left ? node->left : node->right);
            }
            Node *succ = find_min(node->right.get());
            node->data = succ->data;
            node->right = remove_helper(std::move(node->right), succ->data, deleted);
        }

        if (!node) return nullptr;

        update_height(node.get());
        int bf = get_balance(node.get());

        // LL
        if (bf > 1 && get_balance(node->left.get()) >= 0) {
            return rotate_right(std::move(node));
        }
        // LR
        if (bf > 1 && get_balance(node->left.get()) < 0) {
            node->left = rotate_left(std::move(node->left));
            return rotate_right(std::move(node));
        }
        // RR
        if (bf < -1 && get_balance(node->right.get()) <= 0) {
            return rotate_left(std::move(node));
        }
        // RL
        if (bf < -1 && get_balance(node->right.get()) > 0) {
            node->right = rotate_right(std::move(node->right));
            return rotate_left(std::move(node));
        }

        return node;
    }

    std::unique_ptr<Node> copy_node(const Node *src) {
        if (!src) return nullptr;
        auto copy = std::make_unique<Node>(src->data);
        copy->height = src->height;
        copy->left = copy_node(src->left.get());
        copy->right = copy_node(src->right.get());
        return copy;
    }

    void inorder_helper(const Node *node, std::vector<T> &out) const {
        if (!node) return;
        inorder_helper(node->left.get(), out);
        out.push_back(node->data);
        inorder_helper(node->right.get(), out);
    }
};

} // namespace ds

#endif // DS_AVL_TREE_HPP

