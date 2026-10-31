#ifndef DATA_STRUCTURES_LRU_CACHE_HPP
#define DATA_STRUCTURES_LRU_CACHE_HPP

#include <list>
#include <unordered_map>
#include <optional>
#include <cstddef>
#include <stdexcept>

namespace ds {

template <typename Key, typename Value>
class LRUCache {
public:
    struct Entry {
        Key key;
        Value value;
    };

    explicit LRUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than zero");
        }
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return items_.size();
    }

    [[nodiscard]] bool empty() const noexcept {
        return items_.empty();
    }

    std::optional<Value> get(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            return std::nullopt;
        }

        // Move the accessed node to the front (MRU position)
        items_.splice(items_.begin(), items_, it->second);
        return it->second->value;
    }

    void put(const Key& key, Value value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->value = std::move(value);
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        if (items_.size() >= capacity_) {
            // Evict LRU element (back of list)
            auto lru_it = std::prev(items_.end());
            map_.erase(lru_it->key);
            items_.pop_back();
        }

        items_.push_front(Entry{key, std::move(value)});
        map_[key] = items_.begin();
    }

    bool contains(const Key& key) const {
        return map_.find(key) != map_.end();
    }

    void clear() noexcept {
        items_.clear();
        map_.clear();
    }

private:
    std::size_t capacity_;
    std::list<Entry> items_;
    std::unordered_map<Key, typename std::list<Entry>::iterator> map_;
};

} // namespace ds

#endif // DATA_STRUCTURES_LRU_CACHE_HPP
