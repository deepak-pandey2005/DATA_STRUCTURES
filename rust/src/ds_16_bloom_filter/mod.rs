use std::hash::{Hash, Hasher};
use std::collections::hash_map::DefaultHasher;

/// A space-efficient probabilistic Bloom Filter for set membership testing.
pub struct BloomFilter {
    bits: Vec<u64>,
    num_bits: usize,
    num_hashes: usize,
}

impl BloomFilter {
    /// Creates a new `BloomFilter` with the given bit capacity and number of hash iterations.
    ///
    /// # Panics
    /// Panics if `num_bits == 0` or `num_hashes == 0`.
    pub fn new(num_bits: usize, num_hashes: usize) -> Self {
        assert!(num_bits > 0 && num_hashes > 0, "num_bits and num_hashes must be positive");
        let words = (num_bits + 63) / 64;
        Self {
            bits: vec![0; words],
            num_bits,
            num_hashes,
        }
    }

    /// Returns the number of bits in the filter.
    pub fn num_bits(&self) -> usize {
        self.num_bits
    }

    /// Returns the number of hash functions used.
    pub fn num_hashes(&self) -> usize {
        self.num_hashes
    }

    /// Inserts an item into the Bloom Filter.
    pub fn insert<T: Hash + ?Sized>(&mut self, item: &T) {
        let (h1, h2) = self.hashes(item);
        for i in 0..self.num_hashes {
            let bit_idx = ((h1.wrapping_add((i as u64).wrapping_mul(h2))) as usize) % self.num_bits;
            let word_idx = bit_idx / 64;
            let bit_pos = bit_idx % 64;
            self.bits[word_idx] |= 1 << bit_pos;
        }
    }

    /// Checks if an item might be in the set.
    ///
    /// Returns `false` if the item is definitively NOT in the set (zero false negatives).
    /// Returns `true` if the item might be in the set (small false positive probability).
    pub fn contains<T: Hash + ?Sized>(&self, item: &T) -> bool {
        let (h1, h2) = self.hashes(item);
        for i in 0..self.num_hashes {
            let bit_idx = ((h1.wrapping_add((i as u64).wrapping_mul(h2))) as usize) % self.num_bits;
            let word_idx = bit_idx / 64;
            let bit_pos = bit_idx % 64;
            if (self.bits[word_idx] & (1 << bit_pos)) == 0 {
                return false;
            }
        }
        true
    }

    /// Resets all bits in the filter to zero.
    pub fn clear(&mut self) {
        self.bits.fill(0);
    }

    fn hashes<T: Hash + ?Sized>(&self, item: &T) -> (u64, u64) {
        let mut hasher = DefaultHasher::new();
        item.hash(&mut hasher);
        let h1 = hasher.finish();

        // Murmur-style 64-bit avalanche mixer
        let mut h2 = h1 ^ (h1 >> 33);
        h2 = h2.wrapping_mul(0xff51afd7ed558ccd);
        h2 ^= h2 >> 33;
        h2 = h2.wrapping_mul(0xc4ceb9fe1a85ec53);
        h2 ^= h2 >> 33;
        if h2 == 0 {
            h2 = 1;
        }

        (h1, h2)
    }
}
