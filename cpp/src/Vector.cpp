#include "Vector.hpp"
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <string>

namespace ds {

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(std::size_t initial_cap) : data_(nullptr), size_(0), capacity_(initial_cap) {
    if (capacity_ > 0) {
        data_ = allocate(capacity_);
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) : data_(nullptr), size_(0), capacity_(init.size()) {
    if (capacity_ > 0) {
        data_ = allocate(capacity_);
        for (const auto &item : init) {
            push_back(item);
        }
    }
}

template <typename T>
Vector<T>::~Vector() {
    clear();
    deallocate(data_);
}

template <typename T>
Vector<T>::Vector(const Vector &other) : data_(nullptr), size_(0), capacity_(other.size_) {
    if (capacity_ > 0) {
        data_ = allocate(capacity_);
        for (std::size_t i = 0; i < other.size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
        size_ = other.size_;
    }
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
Vector<T>::Vector(Vector &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
    if (this != &other) {
        clear();
        deallocate(data_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
void Vector<T>::swap(Vector &other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename T>
void Vector<T>::push_back(const T &val) {
    ensure_capacity(size_ + 1);
    new (data_ + size_) T(val);
    ++size_;
}

template <typename T>
void Vector<T>::push_back(T &&val) {
    ensure_capacity(size_ + 1);
    new (data_ + size_) T(std::move(val));
    ++size_;
}

template <typename T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::underflow_error("Vector is empty, cannot pop_back");
    }
    --size_;
    data_[size_].~T();
}

template <typename T>
T &Vector<T>::operator[](std::size_t index) noexcept {
    return data_[index];
}

template <typename T>
const T &Vector<T>::operator[](std::size_t index) const noexcept {
    return data_[index];
}

template <typename T>
T &Vector<T>::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Vector index out of range");
    }
    return data_[index];
}

template <typename T>
const T &Vector<T>::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Vector index out of range");
    }
    return data_[index];
}

template <typename T>
T &Vector<T>::front() {
    if (empty()) throw std::underflow_error("Vector is empty");
    return data_[0];
}

template <typename T>
const T &Vector<T>::front() const {
    if (empty()) throw std::underflow_error("Vector is empty");
    return data_[0];
}

template <typename T>
T &Vector<T>::back() {
    if (empty()) throw std::underflow_error("Vector is empty");
    return data_[size_ - 1];
}

template <typename T>
const T &Vector<T>::back() const {
    if (empty()) throw std::underflow_error("Vector is empty");
    return data_[size_ - 1];
}

template <typename T>
void Vector<T>::insert(std::size_t index, const T &val) {
    if (index > size_) throw std::out_of_range("Insert index out of range");
    ensure_capacity(size_ + 1);
    if (index == size_) {
        new (data_ + size_) T(val);
    } else {
        new (data_ + size_) T(std::move(data_[size_ - 1]));
        for (std::size_t i = size_ - 1; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index] = val;
    }
    ++size_;
}

template <typename T>
void Vector<T>::erase(std::size_t index) {
    if (index >= size_) throw std::out_of_range("Erase index out of range");
    for (std::size_t i = index; i + 1 < size_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    data_[size_].~T();
}

template <typename T>
void Vector<T>::reserve(std::size_t new_cap) {
    if (new_cap <= capacity_) return;
    T *new_data = allocate(new_cap);
    for (std::size_t i = 0; i < size_; ++i) {
        new (new_data + i) T(std::move(data_[i]));
        data_[i].~T();
    }
    deallocate(data_);
    data_ = new_data;
    capacity_ = new_cap;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (size_ == capacity_) return;
    if (size_ == 0) {
        clear();
        deallocate(data_);
        data_ = nullptr;
        capacity_ = 0;
        return;
    }
    T *new_data = allocate(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        new (new_data + i) T(std::move(data_[i]));
        data_[i].~T();
    }
    deallocate(data_);
    data_ = new_data;
    capacity_ = size_;
}

template <typename T>
void Vector<T>::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = 0;
}

template <typename T>
std::size_t Vector<T>::size() const noexcept { return size_; }

template <typename T>
std::size_t Vector<T>::capacity() const noexcept { return capacity_; }

template <typename T>
bool Vector<T>::empty() const noexcept { return size_ == 0; }

template <typename T>
T *Vector<T>::data() noexcept { return data_; }

template <typename T>
const T *Vector<T>::data() const noexcept { return data_; }

template <typename T>
T *Vector<T>::begin() noexcept { return data_; }

template <typename T>
const T *Vector<T>::begin() const noexcept { return data_; }

template <typename T>
T *Vector<T>::end() noexcept { return data_ + size_; }

template <typename T>
const T *Vector<T>::end() const noexcept { return data_ + size_; }

template <typename T>
const T *Vector<T>::cbegin() const noexcept { return data_; }

template <typename T>
const T *Vector<T>::cend() const noexcept { return data_ + size_; }

template <typename T>
T *Vector<T>::allocate(std::size_t n) {
    return static_cast<T *>(::operator new(n * sizeof(T)));
}

template <typename T>
void Vector<T>::deallocate(T *p) noexcept {
    ::operator delete(p);
}

template <typename T>
void Vector<T>::ensure_capacity(std::size_t min_cap) {
    if (min_cap > capacity_) {
        std::size_t next_cap = (capacity_ == 0) ? 8 : capacity_ * 2;
        if (next_cap < min_cap) next_cap = min_cap;
        reserve(next_cap);
    }
}

// Explicit template instantiations
template class Vector<int>;
template class Vector<double>;
template class Vector<char>;
template class Vector<std::string>;

} // namespace ds
