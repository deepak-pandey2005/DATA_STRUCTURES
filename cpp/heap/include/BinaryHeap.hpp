/**
 * @file BinaryHeap.hpp
 * @brief Modern C++20 templated Binary Heap (Priority Queue).
 *
 * Implements a generic binary min/max heap backed by std::vector.
 * Defaults to max-heap via std::less<T>, matching std::priority_queue semantics.
 */

#ifndef DS_BINARY_HEAP_HPP
#define DS_BINARY_HEAP_HPP

#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>
#include <cstddef>

namespace ds {

template <typename T, typename Compare = std::less<T>>
class BinaryHeap {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T &;
    using const_reference = const T &;

    BinaryHeap(const Compare &comp = Compare()) : comp_(comp) {}

    template <typename InputIt>
    BinaryHeap(InputIt first, InputIt last, const Compare &comp = Compare())
        : data_(first, last), comp_(comp) {
        heapify();
    }

    ~BinaryHeap() = default;
    BinaryHeap(const BinaryHeap &) = default;
    BinaryHeap &operator=(const BinaryHeap &) = default;
    BinaryHeap(BinaryHeap &&) noexcept = default;
    BinaryHeap &operator=(BinaryHeap &&) noexcept = default;

    /**
     * @brief Accesses the top element (highest priority according to Compare).
     * @throws std::out_of_range if heap is empty.
     */
    [[nodiscard]] const_reference top() const {
        if (empty()) {
            throw std::out_of_range("BinaryHeap::top(): heap is empty");
        }
        return data_.front();
    }

    /**
     * @brief Inserts a new element into the heap in O(log n).
     */
    void push(const T &value) {
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void push(T &&value) {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    /**
     * @brief Emplaces a new element into the heap in O(log n).
     */
    template <typename... Args>
    void emplace(Args &&...args) {
        data_.emplace_back(std::forward<Args>(args)...);
        sift_up(data_.size() - 1);
    }

    /**
     * @brief Removes the top element from the heap in O(log n).
     * @throws std::out_of_range if heap is empty.
     */
    void pop() {
        if (empty()) {
            throw std::out_of_range("BinaryHeap::pop(): heap is empty");
        }
        data_.front() = std::move(data_.back());
        data_.pop_back();
        if (!empty()) {
            sift_down(0);
        }
    }

    [[nodiscard]] size_type size() const noexcept {
        return data_.size();
    }

    [[nodiscard]] bool empty() const noexcept {
        return data_.empty();
    }

    void clear() noexcept {
        data_.clear();
    }

private:
    std::vector<T> data_;
    Compare comp_;

    void sift_up(size_type index) {
        while (index > 0) {
            size_type parent = (index - 1) / 2;
            // If data_[parent] < data_[index] according to comp_, swap
            if (comp_(data_[parent], data_[index])) {
                std::swap(data_[parent], data_[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(size_type index) {
        size_type n = data_.size();
        while (true) {
            size_type left = 2 * index + 1;
            size_type right = 2 * index + 2;
            size_type best = index;

            if (left < n && comp_(data_[best], data_[left])) {
                best = left;
            }
            if (right < n && comp_(data_[best], data_[right])) {
                best = right;
            }

            if (best != index) {
                std::swap(data_[index], data_[best]);
                index = best;
            } else {
                break;
            }
        }
    }

    void heapify() {
        if (data_.size() <= 1) return;
        for (std::size_t i = data_.size() / 2; i > 0; --i) {
            sift_down(i - 1);
        }
    }
};

} // namespace ds

#endif // DS_BINARY_HEAP_HPP

