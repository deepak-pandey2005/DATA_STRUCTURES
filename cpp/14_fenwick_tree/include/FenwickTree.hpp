/**
 * @file FenwickTree.hpp
 * @brief Modern C++20 templated Fenwick Tree (Binary Indexed Tree).
 */

#ifndef DS_FENWICK_TREE_HPP
#define DS_FENWICK_TREE_HPP

#include <vector>
#include <cstddef>
#include <stdexcept>

namespace ds {

template <typename T = int>
class FenwickTree {
public:
    using size_type = std::size_t;

    explicit FenwickTree(size_type n) : size_(n), tree_(n + 1, T{}) {}

    explicit FenwickTree(const std::vector<T> &arr) : size_(arr.size()), tree_(arr.size() + 1, T{}) {
        for (size_type i = 0; i < arr.size(); ++i) {
            add(i, arr[i]);
        }
    }

    ~FenwickTree() = default;
    FenwickTree(const FenwickTree &) = default;
    FenwickTree &operator=(const FenwickTree &) = default;
    FenwickTree(FenwickTree &&) noexcept = default;
    FenwickTree &operator=(FenwickTree &&) noexcept = default;

    /**
     * @brief Adds delta to element at 0-based index in O(log n).
     */
    void add(size_type index, const T &delta) {
        if (index >= size_) {
            throw std::out_of_range("FenwickTree::add: index out of bounds");
        }
        for (size_type i = index + 1; i <= size_; i += (i & (-i))) {
            tree_[i] += delta;
        }
    }

    /**
     * @brief Computes prefix sum of range [0, index] in O(log n).
     */
    [[nodiscard]] T query(size_type index) const {
        if (index >= size_) {
            throw std::out_of_range("FenwickTree::query: index out of bounds");
        }
        T sum = T{};
        for (size_type i = index + 1; i > 0; i -= (i & (-i))) {
            sum += tree_[i];
        }
        return sum;
    }

    /**
     * @brief Computes range sum of range [left, right] inclusive in O(log n).
     */
    [[nodiscard]] T range_query(size_type left, size_type right) const {
        if (left > right || right >= size_) {
            throw std::out_of_range("FenwickTree::range_query: range out of bounds");
        }
        T sum_r = query(right);
        T sum_l = (left > 0) ? query(left - 1) : T{};
        return sum_r - sum_l;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

private:
    size_type size_;
    std::vector<T> tree_;
};

} // namespace ds

#endif // DS_FENWICK_TREE_HPP
