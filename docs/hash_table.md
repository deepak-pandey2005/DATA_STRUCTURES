# Hash Table (Separate Chaining)

A **Hash Table** (or Hash Map) is an associative data structure that maps **keys** to **values**. It provides amortized $O(1)$ time complexity for lookup, insertion, and deletion operations by computing an index from the key via a hash function.

---

## 1. Core Mechanics & Architecture

### Hash Function & Bucket Indexing
1. **Hash Code:** The key is passed through a deterministic hash function (such as **FNV-1a**) yielding a 64-bit integer:
   $$h = \text{hash}(\text{key})$$
2. **Bucket Mapping:** The hash code is mapped to an internal bucket index using the modulo operator:
   $$\text{index} = h \pmod{\text{capacity}}$$

### Collision Resolution: Separate Chaining
When distinct keys hash to the same bucket index ($h_1 \pmod N = h_2 \pmod N$), a collision occurs.
- Each bucket is the head of a **singly linked list** of key-value nodes.
- Colliding keys are chained sequentially inside that bucket.

```
Buckets (Array of Heads):
[0] -> NULL
[1] -> ["apple": 5] -> ["banana": 12] -> NULL  (Collision handled by chaining)
[2] -> NULL
[3] -> ["orange": 8] -> NULL
```

### Load Factor & Dynamic Rehashing
- **Load Factor ($\alpha$):** Defined as $\alpha = \frac{\text{size}}{\text{capacity}}$.
- When $\alpha \ge 0.75$, the hash table allocates a larger bucket array (typically $2 \times \text{capacity}$) and **rehashes** all existing entries into their new bucket positions to prevent degradation from $O(1)$ to $O(n)$.

---

## 2. Operations & Complexity Analysis

| Operation | Average Case | Worst Case (all collide) | Auxiliary Space | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Insert / Put** | $O(1)$ amortized | $O(n)$ | $O(1)$ | Hash key, find bucket, update or prepend node. Rehash if $\alpha \ge 0.75$. |
| **Lookup / Get** | $O(1)$ | $O(n)$ | $O(1)$ | Hash key, traverse bucket list to find matching key. |
| **Delete / Remove** | $O(1)$ | $O(n)$ | $O(1)$ | Hash key, unlink and free target node from bucket list. |
| **Clear** | $O(n)$ | $O(n)$ | $O(1)$ | Free all chain nodes and reset buckets. |

---

## 3. Language Implementation Details

- **C (`c/hash_table/`)**: String-key associative table with FNV-1a hashing, heap-allocated dynamic bucket resizing, and clean memory cleanup.
- **C++ (`cpp/hash_table/`)**: Templated `HashTable<Key, Value, Hash>` using modern move semantics, forward iterators, and custom hashers.
- **Rust (`rust/src/hash_table/`)**: Generic `HashTable<K: Hash + Eq, V>` safe chaining implementation with automated bucket expansion.

