#ifndef CPP_DOUBLY_LINKED_LIST_HPP
#define CPP_DOUBLY_LINKED_LIST_HPP

#include <cstddef>

namespace ds {

template <typename T>
class DoublyLinkedList {
public:
    struct Node {
        T data;
        Node *prev;
        Node *next;

        explicit Node(const T &val);
        explicit Node(T &&val);
    };

    class Iterator {
    public:
        using value_type = T;
        using reference = T &;
        using pointer = T *;

        explicit Iterator(Node *node);
        reference operator*() const;
        pointer operator->() const;
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator--();
        Iterator operator--(int);
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;

    private:
        Node *current_;
    };

    DoublyLinkedList();
    ~DoublyLinkedList();

    // Copy semantics
    DoublyLinkedList(const DoublyLinkedList &other);
    DoublyLinkedList &operator=(const DoublyLinkedList &other);

    // Move semantics
    DoublyLinkedList(DoublyLinkedList &&other) noexcept;
    DoublyLinkedList &operator=(DoublyLinkedList &&other) noexcept;

    void swap(DoublyLinkedList &other) noexcept;

    void push_front(const T &val);
    void push_front(T &&val);
    void push_back(const T &val);
    void push_back(T &&val);

    T pop_front();
    T pop_back();

    void insert_at(std::size_t index, const T &val);
    T remove_at(std::size_t index);
    bool remove_first(const T &val);

    T &get(std::size_t index);
    const T &get(std::size_t index) const;
    T &operator[](std::size_t index);
    const T &operator[](std::size_t index) const;

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    bool contains(const T &val) const;
    void clear() noexcept;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    Iterator begin() noexcept;
    Iterator end() noexcept;

private:
    Node *head_;
    Node *tail_;
    std::size_t size_;
};

} // namespace ds

#endif // CPP_DOUBLY_LINKED_LIST_HPP
