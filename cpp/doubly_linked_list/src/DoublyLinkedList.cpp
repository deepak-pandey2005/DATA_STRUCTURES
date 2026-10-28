#include "DoublyLinkedList.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace ds {

// Node implementations
template <typename T>
DoublyLinkedList<T>::Node::Node(const T &val) : data(val), prev(nullptr), next(nullptr) {}

template <typename T>
DoublyLinkedList<T>::Node::Node(T &&val) : data(std::move(val)), prev(nullptr), next(nullptr) {}

// Iterator implementations
template <typename T>
DoublyLinkedList<T>::Iterator::Iterator(Node *node) : current_(node) {}

template <typename T>
typename DoublyLinkedList<T>::Iterator::reference DoublyLinkedList<T>::Iterator::operator*() const {
    return current_->data;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator::pointer DoublyLinkedList<T>::Iterator::operator->() const {
    return &current_->data;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator &DoublyLinkedList<T>::Iterator::operator++() {
    if (current_) current_ = current_->next;
    return *this;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator &DoublyLinkedList<T>::Iterator::operator--() {
    if (current_) current_ = current_->prev;
    return *this;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

template <typename T>
bool DoublyLinkedList<T>::Iterator::operator==(const Iterator &other) const {
    return current_ == other.current_;
}

template <typename T>
bool DoublyLinkedList<T>::Iterator::operator!=(const Iterator &other) const {
    return current_ != other.current_;
}

// DoublyLinkedList implementations
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList &other)
    : head_(nullptr), tail_(nullptr), size_(0) {
    Node *curr = other.head_;
    while (curr) {
        push_back(curr->data);
        curr = curr->next;
    }
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(const DoublyLinkedList &other) {
    if (this != &other) {
        DoublyLinkedList temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList &&other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(DoublyLinkedList &&other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void DoublyLinkedList<T>::swap(DoublyLinkedList &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
}

template <typename T>
void DoublyLinkedList<T>::push_front(const T &val) {
    Node *node = new Node(val);
    node->next = head_;
    if (head_) {
        head_->prev = node;
    } else {
        tail_ = node;
    }
    head_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::push_front(T &&val) {
    Node *node = new Node(std::move(val));
    node->next = head_;
    if (head_) {
        head_->prev = node;
    } else {
        tail_ = node;
    }
    head_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::push_back(const T &val) {
    Node *node = new Node(val);
    node->prev = tail_;
    if (tail_) {
        tail_->next = node;
    } else {
        head_ = node;
    }
    tail_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::push_back(T &&val) {
    Node *node = new Node(std::move(val));
    node->prev = tail_;
    if (tail_) {
        tail_->next = node;
    } else {
        head_ = node;
    }
    tail_ = node;
    ++size_;
}

template <typename T>
T DoublyLinkedList<T>::pop_front() {
    if (empty()) throw std::underflow_error("List is empty");
    Node *old_head = head_;
    T val = std::move(old_head->data);
    head_ = head_->next;
    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }
    delete old_head;
    --size_;
    return val;
}

template <typename T>
T DoublyLinkedList<T>::pop_back() {
    if (empty()) throw std::underflow_error("List is empty");
    Node *old_tail = tail_;
    T val = std::move(old_tail->data);
    tail_ = tail_->prev;
    if (tail_) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }
    delete old_tail;
    --size_;
    return val;
}

template <typename T>
void DoublyLinkedList<T>::insert_at(std::size_t index, const T &val) {
    if (index > size_) throw std::out_of_range("Insert index out of range");
    if (index == 0) {
        push_front(val);
        return;
    }
    if (index == size_) {
        push_back(val);
        return;
    }

    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }

    Node *node = new Node(val);
    node->prev = curr->prev;
    node->next = curr;
    curr->prev->next = node;
    curr->prev = node;
    ++size_;
}

template <typename T>
T DoublyLinkedList<T>::remove_at(std::size_t index) {
    if (index >= size_) throw std::out_of_range("Remove index out of range");
    if (index == 0) return pop_front();
    if (index == size_ - 1) return pop_back();

    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }

    T val = std::move(curr->data);
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    --size_;
    return val;
}

template <typename T>
bool DoublyLinkedList<T>::remove_first(const T &val) {
    Node *curr = head_;
    while (curr) {
        if (curr->data == val) {
            if (curr->prev) curr->prev->next = curr->next;
            else head_ = curr->next;

            if (curr->next) curr->next->prev = curr->prev;
            else tail_ = curr->prev;

            delete curr;
            --size_;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

template <typename T>
T &DoublyLinkedList<T>::get(std::size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");
    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr->data;
}

template <typename T>
const T &DoublyLinkedList<T>::get(std::size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");
    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr->data;
}

template <typename T>
T &DoublyLinkedList<T>::operator[](std::size_t index) {
    return get(index);
}

template <typename T>
const T &DoublyLinkedList<T>::operator[](std::size_t index) const {
    return get(index);
}

template <typename T>
T &DoublyLinkedList<T>::front() {
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data;
}

template <typename T>
const T &DoublyLinkedList<T>::front() const {
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data;
}

template <typename T>
T &DoublyLinkedList<T>::back() {
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data;
}

template <typename T>
const T &DoublyLinkedList<T>::back() const {
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data;
}

template <typename T>
bool DoublyLinkedList<T>::contains(const T &val) const {
    Node *curr = head_;
    while (curr) {
        if (curr->data == val) return true;
        curr = curr->next;
    }
    return false;
}

template <typename T>
void DoublyLinkedList<T>::clear() noexcept {
    Node *curr = head_;
    while (curr) {
        Node *next = curr->next;
        delete curr;
        curr = next;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}

template <typename T>
std::size_t DoublyLinkedList<T>::size() const noexcept { return size_; }

template <typename T>
bool DoublyLinkedList<T>::empty() const noexcept { return size_ == 0; }

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::begin() noexcept {
    return Iterator(head_);
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::end() noexcept {
    return Iterator(nullptr);
}

// Explicit template instantiations
template class DoublyLinkedList<int>;
template class DoublyLinkedList<double>;
template class DoublyLinkedList<char>;
template class DoublyLinkedList<std::string>;

} // namespace ds
