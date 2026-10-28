/**
 * @file HashTable.hpp
 * @brief Modern C++20 Templated Hash Table using separate chaining.
 */

#ifndef DS_HASH_TABLE_HPP
#define DS_HASH_TABLE_HPP

#include <vector>
#include <list>
#include <functional>
#include <utility>
#include <optional>
#include <stdexcept>
#include <cstddef>

namespace ds {

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const Key, Value>;
    using size_type = std::size_t;

    explicit HashTable(size_type initial_cap = 16, float max_load_factor = 0.75f)
        : buckets_(initial_cap > 0 ? initial_cap : 16),
          size_(0),
          max_load_factor_(max_load_factor),
          hasher_(Hash()) {}

    ~HashTable() = default;
    HashTable(const HashTable &) = default;
    HashTable &operator=(const HashTable &) = default;
    HashTable(HashTable &&) noexcept = default;
    HashTable &operator=(HashTable &&) noexcept = default;

    /**
     * @brief Inserts or updates a key-value pair in O(1) amortized.
     */
    bool insert_or_assign(const Key &key, const Value &val) {
        check_rehash();
        size_type idx = get_bucket_index(key);
        for (auto &entry : buckets_[idx]) {
            if (entry.first == key) {
                entry.second = val;
                return false; // updated
            }
        }
        buckets_[idx].emplace_back(key, val);
        size_++;
        return true; // inserted
    }

    /**
     * @brief Looks up a key, returning a pointer to value if found, or nullptr.
     */
    Value *find(const Key &key) {
        if (size_ == 0) return nullptr;
        size_type idx = get_bucket_index(key);
        for (auto &entry : buckets_[idx]) {
            if (entry.first == key) {
                return &entry.second;
            }
        }
        return nullptr;
    }

    const Value *find(const Key &key) const {
        if (size_ == 0) return nullptr;
        size_type idx = get_bucket_index(key);
        for (const auto &entry : buckets_[idx]) {
            if (entry.first == key) {
                return &entry.second;
            }
        }
        return nullptr;
    }

    [[nodiscard]] bool contains(const Key &key) const {
        return find(key) != nullptr;
    }

    /**
     * @brief Subscript operator: retrieves reference to value or inserts default if not found.
     */
    Value &operator[](const Key &key) {
        check_rehash();
        size_type idx = get_bucket_index(key);
        for (auto &entry : buckets_[idx]) {
            if (entry.first == key) {
                return entry.second;
            }
        }
        buckets_[idx].emplace_back(key, Value{});
        size_++;
        return buckets_[idx].back().second;
    }

    /**
     * @brief Erases an entry matching key in O(1) amortized.
     */
    bool erase(const Key &key) {
        if (size_ == 0) return false;
        size_type idx = get_bucket_index(key);
        auto &bucket = buckets_[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                size_--;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    void clear() noexcept {
        for (auto &bucket : buckets_) {
            bucket.clear();
        }
        size_ = 0;
    }

private:
    std::vector<std::list<std::pair<Key, Value>>> buckets_;
    size_type size_;
    float max_load_factor_;
    Hash hasher_;

    size_type get_bucket_index(const Key &key) const {
        return hasher_(key) % buckets_.size();
    }

    void check_rehash() {
        if (static_cast<float>(size_ + 1) / static_cast<float>(buckets_.size()) >= max_load_factor_) {
            rehash(buckets_.size() * 2);
        }
    }

    void rehash(size_type new_capacity) {
        std::vector<std::list<std::pair<Key, Value>>> new_buckets(new_capacity);
        for (auto &bucket : buckets_) {
            for (auto &entry : bucket) {
                size_type new_idx = hasher_(entry.first) % new_capacity;
                new_buckets[new_idx].push_back(std::move(entry));
            }
        }
        buckets_ = std::move(new_buckets);
    }
};

} // namespace ds

#endif // DS_HASH_TABLE_HPP
