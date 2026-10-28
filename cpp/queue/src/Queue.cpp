#include "Queue.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace ds {

template <typename T>
Queue<T>::Queue(std::size_t initial_cap)
    : buffer_(nullptr), head_(0), tail_(0), size_(0), capacity_(initial_cap) {
    if (capacity_ > 0) {
        buffer_ = allocate(capacity_);
    }
}

template <typename T>
Queue<T>::~Queue() {
    clear();
    deallocate(buffer_);
}

template <typename T>
Queue<T>::Queue(const Queue &other)
    : buffer_(nullptr), head_(0), tail_(0), size_(0), capacity_(other.size_) {
    if (capacity_ > 0) {
        buffer_ = allocate(capacity_);
        for (std::size_t i = 0; i < other.size_; ++i) {
            std::size_t idx = (other.head_ + i) % other.capacity_;
            new (buffer_ + i) T(other.buffer_[idx]);
        }
        size_ = other.size_;
        head_ = 0;
        tail_ = size_;
    }
}

template <typename T>
Queue<T> &Queue<T>::operator=(const Queue &other) {
    if (this != &other) {
        Queue temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
Queue<T>::Queue(Queue &&other) noexcept
    : buffer_(other.buffer_), head_(other.head_), tail_(other.tail_),
      size_(other.size_), capacity_(other.capacity_) {
    other.buffer_ = nullptr;
    other.head_ = other.tail_ = other.size_ = other.capacity_ = 0;
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue &&other) noexcept {
    if (this != &other) {
        clear();
        deallocate(buffer_);
        buffer_ = other.buffer_;
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.buffer_ = nullptr;
        other.head_ = other.tail_ = other.size_ = other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
void Queue<T>::swap(Queue &other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename T>
void Queue<T>::enqueue(const T &val) {
    if (size_ >= capacity_) grow();
    new (buffer_ + tail_) T(val);
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
}

template <typename T>
void Queue<T>::enqueue(T &&val) {
    if (size_ >= capacity_) grow();
    new (buffer_ + tail_) T(std::move(val));
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
}

template <typename T>
T Queue<T>::dequeue() {
    if (empty()) throw std::underflow_error("Queue is empty, cannot dequeue");
    T val = std::move(buffer_[head_]);
    buffer_[head_].~T();
    head_ = (head_ + 1) % capacity_;
    --size_;
    return val;
}

template <typename T>
T &Queue<T>::front() {
    if (empty()) throw std::underflow_error("Queue is empty");
    return buffer_[head_];
}

template <typename T>
const T &Queue<T>::front() const {
    if (empty()) throw std::underflow_error("Queue is empty");
    return buffer_[head_];
}

template <typename T>
T &Queue<T>::back() {
    if (empty()) throw std::underflow_error("Queue is empty");
    std::size_t last_idx = (tail_ == 0) ? capacity_ - 1 : tail_ - 1;
    return buffer_[last_idx];
}

template <typename T>
const T &Queue<T>::back() const {
    if (empty()) throw std::underflow_error("Queue is empty");
    std::size_t last_idx = (tail_ == 0) ? capacity_ - 1 : tail_ - 1;
    return buffer_[last_idx];
}

template <typename T>
void Queue<T>::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        std::size_t idx = (head_ + i) % capacity_;
        buffer_[idx].~T();
    }
    head_ = 0;
    tail_ = 0;
    size_ = 0;
}

template <typename T>
std::size_t Queue<T>::size() const noexcept { return size_; }

template <typename T>
std::size_t Queue<T>::capacity() const noexcept { return capacity_; }

template <typename T>
bool Queue<T>::empty() const noexcept { return size_ == 0; }

template <typename T>
T *Queue<T>::allocate(std::size_t n) {
    return static_cast<T *>(::operator new(n * sizeof(T)));
}

template <typename T>
void Queue<T>::deallocate(T *p) noexcept {
    ::operator delete(p);
}

template <typename T>
void Queue<T>::grow() {
    std::size_t new_cap = (capacity_ == 0) ? 8 : capacity_ * 2;
    T *new_buffer = allocate(new_cap);

    for (std::size_t i = 0; i < size_; ++i) {
        std::size_t idx = (head_ + i) % capacity_;
        new (new_buffer + i) T(std::move(buffer_[idx]));
        buffer_[idx].~T();
    }

    deallocate(buffer_);
    buffer_ = new_buffer;
    head_ = 0;
    tail_ = size_;
    capacity_ = new_cap;
}

// Explicit template instantiations
template class Queue<int>;
template class Queue<double>;
template class Queue<char>;
template class Queue<std::string>;

} // namespace ds
