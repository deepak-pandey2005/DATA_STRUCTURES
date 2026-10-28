#ifndef CPP_BINARY_SEARCH_TREE_HPP
#define CPP_BINARY_SEARCH_TREE_HPP

#include <cstddef>
#include <functional>
#include <vector>

namespace ds {

template <typename T>
class BinarySearchTree {
public:
    struct Node {
        T key;
        Node *left;
        Node *right;

        explicit Node(const T &k);
        explicit Node(T &&k);
    };

    BinarySearchTree();
    ~BinarySearchTree();

    // Copy semantics
    BinarySearchTree(const BinarySearchTree &other);
    BinarySearchTree &operator=(const BinarySearchTree &other);

    // Move semantics
    BinarySearchTree(BinarySearchTree &&other) noexcept;
    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept;

    void swap(BinarySearchTree &other) noexcept;

    bool insert(const T &key);
    bool remove(const T &key);
    bool contains(const T &key) const;

    const T &min() const;
    const T &max() const;

    [[nodiscard]] std::size_t height() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    void clear() noexcept;

    // Traversals
    void inorder(const std::function<void(const T &)> &visitor) const;
    void preorder(const std::function<void(const T &)> &visitor) const;
    void postorder(const std::function<void(const T &)> &visitor) const;
    void level_order(const std::function<void(const T &)> &visitor) const;

    [[nodiscard]] std::vector<T> inorder_keys() const;
    [[nodiscard]] std::vector<T> preorder_keys() const;
    [[nodiscard]] std::vector<T> postorder_keys() const;
    [[nodiscard]] std::vector<T> level_order_keys() const;

private:
    Node *root_;
    std::size_t size_;

    static Node *clone(Node *node);
    static void destroy(Node *node) noexcept;
    static Node *find_min(Node *node);
    static std::size_t height_node(const Node *node);

    Node *insert_node(Node *node, const T &key, bool &inserted);
    Node *remove_node(Node *node, const T &key, bool &removed);

    static void inorder_node(Node *node, const std::function<void(const T &)> &visitor);
    static void preorder_node(Node *node, const std::function<void(const T &)> &visitor);
    static void postorder_node(Node *node, const std::function<void(const T &)> &visitor);
};

} // namespace ds

#endif // CPP_BINARY_SEARCH_TREE_HPP
