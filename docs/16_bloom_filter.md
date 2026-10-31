# Bloom Filter

A **Bloom Filter** is a space-efficient probabilistic data structure used to test set membership. It yields either **"possibly in set"** or **"definitely not in set"**. False positive matches are possible, but false negatives are impossible.

---

## 1. Core Architecture & Bit-Vector Design

A Bloom Filter is composed of:
1. A **bit array** of size $m$, initially all set to `0`.
2. $k$ independent, uniform **hash functions** mapping an input key to one of the $m$ bit array positions $\{0, 1, \dots, m-1\}$.

```
Input: "apple"
  Hash 1("apple") % 16 = 3  ────────┐
  Hash 2("apple") % 16 = 7  ────────┼─► Set bits at indices [3, 7, 12] to 1
  Hash 3("apple") % 16 = 12 ────────┘

Bit Array (m = 16):
Index:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
Bits:  [0] [0] [0] [1] [0] [0] [0] [1] [0] [0] [0] [0] [1] [0] [0] [0]
                    ▲               ▲                   ▲
```

### Operations
- **`insert(key)`**: Pass `key` through all $k$ hash functions to get bit indices $h_1(x), h_2(x), \dots, h_k(x)$ and set each bit to `1`.
- **`contains(key)`**: Pass `key` through all $k$ hash functions.
  - If **any** of the bits at $h_i(x)$ is `0`, the element is **definitely not in the set**.
  - If **all** bits are `1`, the element is **probably in the set** (with a calculable false positive probability $\epsilon$).

### Kirsch-Mitzenmacher Optimization
Instead of computing $k$ separate hash functions, we can simulate $k$ hash functions using just two 64-bit hash values $h_1(x)$ and $h_2(x)$:
$$g_i(x) = (h_1(x) + i \cdot h_2(x)) \pmod m$$
for $i \in \{0, 1, \dots, k-1\}$.

---

## 2. Complexity & Probability Analysis

| Operation | Time Complexity | Auxiliary Space | Description |
| :--- | :---: | :---: | :--- |
| **Insert** | $O(k)$ | $O(m)$ bits | Compute $k$ bit indices and set bits to 1. |
| **Lookup (`contains`)** | $O(k)$ | $O(1)$ | Check $k$ bit indices. Short-circuits on first `0`. |
| **Delete** | Not supported | - | Standard Bloom Filters do not support deletion. |

### False Positive Probability $\epsilon$:
$$\epsilon \approx \left(1 - e^{-kn/m}\right)^k$$
Optimal number of hash functions for a given bit array size $m$ and expected items $n$:
$$k = \frac{m}{n} \ln 2 \approx 0.693 \cdot \frac{m}{n}$$

---

## 3. Language Implementation Highlights

- **C (`c/16_bloom_filter/`)**: Flat dynamic bit array allocated in 64-bit words (`uint64_t[]`), with bitwise shifts and dual-hash simulation via Murmur/FNV-style hashing.
- **C++ (`cpp/16_bloom_filter/`)**: Templated `BloomFilter<Key>` using `std::vector<bool>` or dynamic bitsets with `std::hash`.
- **Rust (`rust/src/ds_16_bloom_filter/`)**: Idiomatic Rust implementation wrapping `Vec<u64>` bit storage with generic `Hash` trait support.
