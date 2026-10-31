#ifndef DATA_STRUCTURES_SKIP_LIST_HPP
#define DATA_STRUCTURES_SKIP_LIST_HPP

#include <vector>
#include <memory>
#include <random>
#include <optional>
#include <cstddef>

namespace ds {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class SkipList {
public:
    static constexpr std::size_t MAX_LEVEL = 16;
    static constexpr float PROBABILITY = 0.5f;

    struct Node {
        Key key;
        Value value;
        std::vector<std::shared_ptr<Node>> forward;

        Node(Key k, Value v, std::size_t level)
            : key(std::move(k)), value(std::move(v)), forward(level, nullptr) {}
    };

    SkipList() : level_(1), size_(0), gen_(std::random_device{}()), dist_(0.0f, 1.0f) {
        head_ = std::make_shared<Node>(Key{}, Value{}, MAX_LEVEL);
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    std::optional<Value> search(const Key& key) const {
        auto curr = head_;
        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (curr->forward[i] && comp_(curr->forward[i]->key, key)) {
                curr = curr->forward[i];
            }
        }
        curr = curr->forward[0];
        if (curr && !comp_(curr->key, key) && !comp_(key, curr->key)) {
            return curr->value;
        }
        return std::nullopt;
    }

    bool insert(Key key, Value value) {
        std::vector<std::shared_ptr<Node>> update(MAX_LEVEL, nullptr);
        auto curr = head_;

        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (curr->forward[i] && comp_(curr->forward[i]->key, key)) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];
        if (curr && !comp_(curr->key, key) && !comp_(key, curr->key)) {
            curr->value = std::move(value);
            return false; // updated existing
        }

        std::size_t new_level = random_level();
        if (new_level > level_) {
            for (std::size_t i = level_; i < new_level; ++i) {
                update[i] = head_;
            }
            level_ = new_level;
        }

        auto new_node = std::make_shared<Node>(std::move(key), std::move(value), new_level);
        for (std::size_t i = 0; i < new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }

        size_++;
        return true;
    }

    bool remove(const Key& key) {
        std::vector<std::shared_ptr<Node>> update(MAX_LEVEL, nullptr);
        auto curr = head_;

        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (curr->forward[i] && comp_(curr->forward[i]->key, key)) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];
        if (!curr || comp_(curr->key, key) || comp_(key, curr->key)) {
            return false;
        }

        for (std::size_t i = 0; i < level_; ++i) {
            if (update[i]->forward[i] != curr) {
                break;
            }
            update[i]->forward[i] = curr->forward[i];
        }

        while (level_ > 1 && head_->forward[level_ - 1] == nullptr) {
            level_--;
        }

        size_--;
        return true;
    }

private:
    std::shared_ptr<Node> head_;
    std::size_t level_;
    std::size_t size_;
    Compare comp_{};
    mutable std::mt19937 gen_;
    mutable std::uniform_real_distribution<float> dist_;

    std::size_t random_level() const {
        std::size_t lvl = 1;
        while (dist_(gen_) < PROBABILITY && lvl < MAX_LEVEL) {
            lvl++;
        }
        return lvl;
    }
};

} // namespace ds

#endif // DATA_STRUCTURES_SKIP_LIST_HPP

