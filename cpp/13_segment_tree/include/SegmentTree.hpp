/**
 * @file SegmentTree.hpp
 * @brief Modern C++20 templated Segment Tree for range associative queries.
 */

#ifndef DS_SEGMENT_TREE_HPP
#define DS_SEGMENT_TREE_HPP

#include <vector>
#include <functional>
#include <cstddef>
#include <stdexcept>

namespace ds {

template <typename T, typename Op = std::plus<T>>
class SegmentTree {
public:
    using size_type = std::size_t;

    SegmentTree(const std::vector<T> &arr, T identity = T{}, const Op &op = Op())
        : n_(arr.size()), identity_(identity), op_(op) {
        if (n_ > 0) {
            tree_.resize(4 * n_, identity_);
            build(arr, 0, 0, n_ - 1);
        }
    }

    ~SegmentTree() = default;
    SegmentTree(const SegmentTree &) = default;
    SegmentTree &operator=(const SegmentTree &) = default;
    SegmentTree(SegmentTree &&) noexcept = default;
    SegmentTree &operator=(SegmentTree &&) noexcept = default;

    /**
     * @brief Queries associative range query [left, right] inclusive in O(log n).
     */
    [[nodiscard]] T query(size_type left, size_type right) const {
        if (left > right || right >= n_) {
            throw std::out_of_range("SegmentTree::query: range out of bounds");
        }
        return query_helper(0, 0, n_ - 1, left, right);
    }

    /**
     * @brief Updates value at index in O(log n).
     */
    void update(size_type index, const T &new_val) {
        if (index >= n_) {
            throw std::out_of_range("SegmentTree::update: index out of bounds");
        }
        update_helper(0, 0, n_ - 1, index, new_val);
    }

    [[nodiscard]] size_type size() const noexcept {
        return n_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return n_ == 0;
    }

private:
    size_type n_;
    T identity_;
    Op op_;
    std::vector<T> tree_;

    void build(const std::vector<T> &arr, size_type node, size_type start, size_type end) {
        if (start == end) {
            tree_[node] = arr[start];
            return;
        }
        size_type mid = start + (end - start) / 2;
        size_type left_child = 2 * node + 1;
        size_type right_child = 2 * node + 2;

        build(arr, left_child, start, mid);
        build(arr, right_child, mid + 1, end);
        tree_[node] = op_(tree_[left_child], tree_[right_child]);
    }

    T query_helper(size_type node, size_type start, size_type end, size_type l, size_type r) const {
        if (r < start || end < l) {
            return identity_;
        }
        if (l <= start && end <= r) {
            return tree_[node];
        }
        size_type mid = start + (end - start) / 2;
        T left_res = query_helper(2 * node + 1, start, mid, l, r);
        T right_res = query_helper(2 * node + 2, mid + 1, end, l, r);
        return op_(left_res, right_res);
    }

    void update_helper(size_type node, size_type start, size_type end, size_type idx, const T &val) {
        if (start == end) {
            tree_[node] = val;
            return;
        }
        size_type mid = start + (end - start) / 2;
        size_type left_child = 2 * node + 1;
        size_type right_child = 2 * node + 2;

        if (idx <= mid) {
            update_helper(left_child, start, mid, idx, val);
        } else {
            update_helper(right_child, mid + 1, end, idx, val);
        }
        tree_[node] = op_(tree_[left_child], tree_[right_child]);
    }
};

} // namespace ds

#endif // DS_SEGMENT_TREE_HPP
