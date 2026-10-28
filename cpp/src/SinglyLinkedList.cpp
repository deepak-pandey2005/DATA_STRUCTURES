#include "SinglyLinkedList.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace ds {

// Node implementations
template <typename T>
SinglyLinkedList<T>::Node::Node(const T &val) : data(val), next(nullptr) {}

template <typename T>
SinglyLinkedList<T>::Node::Node(T &&val) : data(std::move(val)), next(nullptr) {}

// Iterator implementations
template <typename T>
SinglyLinkedList<T>::Iterator::Iterator(Node *node) : current_(node) {}

template <typename T>
typename SinglyLinkedList<T>::Iterator::reference SinglyLinkedList<T>::Iterator::operator*() const {
    return current_->data;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator::pointer SinglyLinkedList<T>::Iterator::operator->() const {
    return &current_->data;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator &SinglyLinkedList<T>::Iterator::operator++() {
    if (current_) current_ = current_->next;
    return *this;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
bool SinglyLinkedList<T>::Iterator::operator==(const Iterator &other) const {
    return current_ == other.current_;
}

template <typename T>
bool SinglyLinkedList<T>::Iterator::operator!=(const Iterator &other) const {
    return current_ != other.current_;
}

// SinglyLinkedList implementations
template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    clear();
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList &other) : head_(nullptr), tail_(nullptr), size_(0) {
    Node *curr = other.head_;
    while (curr) {
        push_back(curr->data);
        curr = curr->next;
    }
}

template <typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(const SinglyLinkedList &other) {
    if (this != &other) {
        SinglyLinkedList temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList &&other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(SinglyLinkedList &&other) noexcept {
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
void SinglyLinkedList<T>::swap(SinglyLinkedList &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
}

template <typename T>
void SinglyLinkedList<T>::push_front(const T &val) {
    Node *node = new Node(val);
    node->next = head_;
    head_ = node;
    if (!tail_) tail_ = head_;
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::push_front(T &&val) {
    Node *node = new Node(std::move(val));
    node->next = head_;
    head_ = node;
    if (!tail_) tail_ = head_;
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::push_back(const T &val) {
    Node *node = new Node(val);
    if (!tail_) {
        head_ = tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::push_back(T &&val) {
    Node *node = new Node(std::move(val));
    if (!tail_) {
        head_ = tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    ++size_;
}

template <typename T>
T SinglyLinkedList<T>::pop_front() {
    if (empty()) throw std::underflow_error("List is empty");
    Node *old_head = head_;
    T val = std::move(old_head->data);
    head_ = head_->next;
    if (!head_) tail_ = nullptr;
    delete old_head;
    --size_;
    return val;
}

template <typename T>
T SinglyLinkedList<T>::pop_back() {
    if (empty()) throw std::underflow_error("List is empty");
    if (size_ == 1) return pop_front();

    Node *curr = head_;
    while (curr->next != tail_) {
        curr = curr->next;
    }
    T val = std::move(tail_->data);
    delete tail_;
    tail_ = curr;
    tail_->next = nullptr;
    --size_;
    return val;
}

template <typename T>
void SinglyLinkedList<T>::insert_at(std::size_t index, const T &val) {
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
    for (std::size_t i = 0; i < index - 1; ++i) {
        curr = curr->next;
    }

    Node *node = new Node(val);
    node->next = curr->next;
    curr->next = node;
    ++size_;
}

template <typename T>
T SinglyLinkedList<T>::remove_at(std::size_t index) {
    if (index >= size_) throw std::out_of_range("Remove index out of range");
    if (index == 0) return pop_front();

    Node *curr = head_;
    for (std::size_t i = 0; i < index - 1; ++i) {
        curr = curr->next;
    }

    Node *to_remove = curr->next;
    T val = std::move(to_remove->data);
    curr->next = to_remove->next;
    if (to_remove == tail_) {
        tail_ = curr;
    }
    delete to_remove;
    --size_;
    return val;
}

template <typename T>
bool SinglyLinkedList<T>::remove_value(const T &val) {
    if (empty()) return false;
    if (head_->data == val) {
        pop_front();
        return true;
    }

    Node *curr = head_;
    while (curr->next && !(curr->next->data == val)) {
        curr = curr->next;
    }

    if (!curr->next) return false;

    Node *to_remove = curr->next;
    curr->next = to_remove->next;
    if (to_remove == tail_) {
        tail_ = curr;
    }
    delete to_remove;
    --size_;
    return true;
}

template <typename T>
T &SinglyLinkedList<T>::get(std::size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");
    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr->data;
}

template <typename T>
const T &SinglyLinkedList<T>::get(std::size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");
    Node *curr = head_;
    for (std::size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr->data;
}

template <typename T>
T &SinglyLinkedList<T>::operator[](std::size_t index) {
    return get(index);
}

template <typename T>
const T &SinglyLinkedList<T>::operator[](std::size_t index) const {
    return get(index);
}

template <typename T>
T &SinglyLinkedList<T>::front() {
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data;
}

template <typename T>
const T &SinglyLinkedList<T>::front() const {
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data;
}

template <typename T>
T &SinglyLinkedList<T>::back() {
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data;
}

template <typename T>
const T &SinglyLinkedList<T>::back() const {
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data;
}

template <typename T>
bool SinglyLinkedList<T>::contains(const T &val) const {
    Node *curr = head_;
    while (curr) {
        if (curr->data == val) return true;
        curr = curr->next;
    }
    return false;
}

template <typename T>
void SinglyLinkedList<T>::reverse() {
    if (size_ <= 1) return;
    Node *prev = nullptr;
    Node *curr = head_;
    Node *next = nullptr;
    tail_ = head_;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head_ = prev;
}

template <typename T>
void SinglyLinkedList<T>::clear() noexcept {
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
std::size_t SinglyLinkedList<T>::size() const noexcept { return size_; }

template <typename T>
bool SinglyLinkedList<T>::empty() const noexcept { return size_ == 0; }

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin() noexcept {
    return Iterator(head_);
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end() noexcept {
    return Iterator(nullptr);
}

// Explicit template instantiations
template class SinglyLinkedList<int>;
template class SinglyLinkedList<double>;
template class SinglyLinkedList<char>;
template class SinglyLinkedList<std::string>;

} // namespace ds
