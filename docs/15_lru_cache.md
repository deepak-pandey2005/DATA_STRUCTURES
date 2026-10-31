# LRU Cache (Least Recently Used Cache)

An **LRU (Least Recently Used) Cache** is a fixed-capacity caching structure that evicts the item that has not been accessed for the longest time when the cache is full.

---

## 1. Core Architecture & Hybrid Design

To achieve strictly **$O(1)$** time complexity for both `get` and `put`, an LRU Cache pairs two foundational data structures:

1. **Hash Table (Map):** Maps keys directly to their corresponding nodes in the doubly linked list for $O(1)$ lookup.
2. **Doubly Linked List:** Maintains the chronological usage ordering of elements with sentinel head and tail nodes.
   - **Most Recently Used (MRU):** Positioned directly after the dummy `head`.
   - **Least Recently Used (LRU):** Positioned directly before the dummy `tail`.

```
  [ Head (Dummy) ] <---> [ Node A (MRU) ] <---> [ Node B ] <---> [ Node C (LRU) ] <---> [ Tail (Dummy) ]
         │                       ▲                                       ▲
         │                       │                                       │
    Hash Map:              "keyA": ptr                             "keyC": ptr
```

### Operations Flow
- **`get(key)`**: Look up node in map. If found, unlink node and move it directly behind dummy `head` (marking it MRU), then return value.
- **`put(key, value)`**:
  - If key exists: update value and promote node to `head`.
  - If key is new and cache is at capacity: evict node right before dummy `tail` (LRU node), remove its key from the hash map, then insert the new node behind `head`.

---

## 2. Complexity Analysis

| Operation | Time Complexity | Auxiliary Space | Description |
| :--- | :---: | :---: | :--- |
| **Get** | $O(1)$ | $O(1)$ | Map lookup + splice node to head of list. |
| **Put** | $O(1)$ | $O(1)$ | Map lookup / insert + list splice (and optional tail eviction). |
| **Eviction** | $O(1)$ | $O(1)$ | Unlink node before tail dummy sentinel and erase from map. |

---

## 3. Language Implementation Highlights

- **C (`c/15_lru_cache/`)**: Fixed capacity with internal hash table and explicit pointer splicing with dummy sentinel nodes.
- **C++ (`cpp/15_lru_cache/`)**: Templated `LRUCache<Key, Value>` using `std::unordered_map` and `std::list` splice iterator manipulation.
- **Rust (`rust/src/ds_15_lru_cache/`)**: Safe Rust implementation using generational index arrays / index-linked doubly linked list or map + order vec.
