#ifndef DATA_STRUCTURES_BLOOM_FILTER_HPP
#define DATA_STRUCTURES_BLOOM_FILTER_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace ds {

template <typename Key = std::string>
class BloomFilter {
public:
    BloomFilter(std::size_t num_bits, std::size_t num_hashes)
        : num_bits_(num_bits), num_hashes_(num_hashes), bits_((num_bits + 63) / 64, 0) {
        if (num_bits == 0 || num_hashes == 0) {
            throw std::invalid_argument("num_bits and num_hashes must be greater than zero");
        }
    }

    [[nodiscard]] std::size_t num_bits() const noexcept {
        return num_bits_;
    }

    [[nodiscard]] std::size_t num_hashes() const noexcept {
        return num_hashes_;
    }

    void add(const Key& key) {
        auto [h1, h2] = hash_key(key);
        for (std::size_t i = 0; i < num_hashes_; ++i) {
            std::size_t bit_idx = (h1 + i * h2) % num_bits_;
            std::size_t word_idx = bit_idx / 64;
            std::size_t bit_pos = bit_idx % 64;
            bits_[word_idx] |= (1ULL << bit_pos);
        }
    }

    [[nodiscard]] bool contains(const Key& key) const {
        auto [h1, h2] = hash_key(key);
        for (std::size_t i = 0; i < num_hashes_; ++i) {
            std::size_t bit_idx = (h1 + i * h2) % num_bits_;
            std::size_t word_idx = bit_idx / 64;
            std::size_t bit_pos = bit_idx % 64;
            if (!(bits_[word_idx] & (1ULL << bit_pos))) {
                return false;
            }
        }
        return true;
    }

    void clear() noexcept {
        std::fill(bits_.begin(), bits_.end(), 0ULL);
    }

private:
    std::size_t num_bits_;
    std::size_t num_hashes_;
    std::vector<std::uint64_t> bits_;

    std::pair<std::uint64_t, std::uint64_t> hash_key(const Key& key) const {
        std::hash<Key> hasher;
        std::uint64_t h1 = static_cast<std::uint64_t>(hasher(key));
        
        // 64-bit Murmur-style mixer for second hash
        std::uint64_t h2 = h1;
        h2 ^= h2 >> 33;
        h2 *= 0xff51afd7ed558ccdULL;
        h2 ^= h2 >> 33;
        h2 *= 0xc4ceb9fe1a85ec53ULL;
        h2 ^= h2 >> 33;
        if (h2 == 0) h2 = 1;

        return {h1, h2};
    }
};

} // namespace ds

#endif // DATA_STRUCTURES_BLOOM_FILTER_HPP
