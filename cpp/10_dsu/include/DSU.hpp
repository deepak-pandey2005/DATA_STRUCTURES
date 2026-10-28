/**
 * @file DSU.hpp
 * @brief Modern C++20 Disjoint Set Union (Union-Find) with path compression and union by rank.
 */

#ifndef DS_DSU_HPP
#define DS_DSU_HPP

#include <vector>
#include <numeric>
#include <cstddef>
#include <stdexcept>

namespace ds {

class DSU {
public:
    using size_type = std::size_t;

    explicit DSU(size_type n) : parent_(n), rank_(n, 0), num_sets_(n) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    ~DSU() = default;
    DSU(const DSU &) = default;
    DSU &operator=(const DSU &) = default;
    DSU(DSU &&) noexcept = default;
    DSU &operator=(DSU &&) noexcept = default;

    /**
     * @brief Finds the canonical root representative of element x with path compression.
     */
    [[nodiscard]] size_type find(size_type x) {
        check_bounds(x);
        if (parent_[x] != x) {
            parent_[x] = find(parent_[x]); // Path compression
        }
        return parent_[x];
    }

    /**
     * @brief Merges sets containing elements x and y using union by rank.
     * @return true if elements were in distinct sets and merged; false if already in same set.
     */
    bool unite(size_type x, size_type y) {
        size_type root_x = find(x);
        size_type root_y = find(y);

        if (root_x == root_y) {
            return false;
        }

        if (rank_[root_x] < rank_[root_y]) {
            parent_[root_x] = root_y;
        } else if (rank_[root_x] > rank_[root_y]) {
            parent_[root_y] = root_x;
        } else {
            parent_[root_y] = root_x;
            rank_[root_x]++;
        }

        num_sets_--;
        return true;
    }

    /**
     * @brief Checks if x and y belong to the same set.
     */
    [[nodiscard]] bool connected(size_type x, size_type y) {
        return find(x) == find(y);
    }

    [[nodiscard]] size_type set_count() const noexcept {
        return num_sets_;
    }

    [[nodiscard]] size_type size() const noexcept {
        return parent_.size();
    }

private:
    std::vector<size_type> parent_;
    std::vector<size_type> rank_;
    size_type num_sets_;

    void check_bounds(size_type x) const {
        if (x >= parent_.size()) {
            throw std::out_of_range("DSU::find: index out of bounds");
        }
    }
};

} // namespace ds

#endif // DS_DSU_HPP

