/**
 * @file Trie.hpp
 * @brief Modern C++20 Trie (Prefix Tree) implementation.
 */

#ifndef DS_TRIE_HPP
#define DS_TRIE_HPP

#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <cstddef>
#include <cctype>

namespace ds {

class Trie {
public:
    Trie() : root_(std::make_unique<Node>()), word_count_(0) {}
    ~Trie() = default;

    Trie(const Trie &other) : root_(std::make_unique<Node>()), word_count_(0) {
        copy_from(other);
    }

    Trie &operator=(const Trie &other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    Trie(Trie &&) noexcept = default;
    Trie &operator=(Trie &&) noexcept = default;

    /**
     * @brief Inserts a word consisting of case-insensitive ASCII letters into the Trie.
     */
    bool insert(std::string_view word) {
        Node *curr = root_.get();
        for (char ch : word) {
            char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            if (lower < 'a' || lower > 'z') return false;
            std::size_t idx = lower - 'a';
            if (!curr->children[idx]) {
                curr->children[idx] = std::make_unique<Node>();
            }
            curr = curr->children[idx].get();
        }
        if (!curr->is_end) {
            curr->is_end = true;
            word_count_++;
            return true;
        }
        return false;
    }

    /**
     * @brief Returns true if exact word is found in the Trie.
     */
    [[nodiscard]] bool search(std::string_view word) const {
        const Node *curr = find_node(word);
        return curr && curr->is_end;
    }

    /**
     * @brief Returns true if there is any word in the Trie starting with prefix.
     */
    [[nodiscard]] bool starts_with(std::string_view prefix) const {
        return find_node(prefix) != nullptr;
    }

    /**
     * @brief Removes a word from the Trie in O(L).
     */
    bool remove(std::string_view word) {
        bool removed = false;
        remove_helper(root_.get(), word, 0, removed);
        if (removed) {
            word_count_--;
        }
        return removed;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return word_count_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return word_count_ == 0;
    }

    void clear() noexcept {
        root_ = std::make_unique<Node>();
        word_count_ = 0;
    }

private:
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{};
        bool is_end = false;
    };

    std::unique_ptr<Node> root_;
    std::size_t word_count_;

    [[nodiscard]] const Node *find_node(std::string_view str) const {
        const Node *curr = root_.get();
        for (char ch : str) {
            char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            if (lower < 'a' || lower > 'z') return nullptr;
            std::size_t idx = lower - 'a';
            if (!curr->children[idx]) return nullptr;
            curr = curr->children[idx].get();
        }
        return curr;
    }

    bool remove_helper(Node *node, std::string_view word, std::size_t depth, bool &removed) {
        if (!node) return false;

        if (depth == word.size()) {
            if (node->is_end) {
                node->is_end = false;
                removed = true;
            }
            return has_no_children(node);
        }

        char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(word[depth])));
        if (lower < 'a' || lower > 'z') return false;
        std::size_t idx = lower - 'a';

        if (remove_helper(node->children[idx].get(), word, depth + 1, removed)) {
            node->children[idx].reset();
            return !node->is_end && has_no_children(node);
        }
        return false;
    }

    static bool has_no_children(const Node *node) {
        for (const auto &child : node->children) {
            if (child) return false;
        }
        return true;
    }

    void copy_helper(Node *dest, const Node *src) {
        dest->is_end = src->is_end;
        for (std::size_t i = 0; i < 26; ++i) {
            if (src->children[i]) {
                dest->children[i] = std::make_unique<Node>();
                copy_helper(dest->children[i].get(), src->children[i].get());
            }
        }
    }

    void copy_from(const Trie &other) {
        copy_helper(root_.get(), other.root_.get());
        word_count_ = other.word_count_;
    }
};

} // namespace ds

#endif // DS_TRIE_HPP
