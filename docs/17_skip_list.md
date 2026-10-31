# Skip List

A **Skip List** is a probabilistic data structure that enables fast search, insertion, and deletion within an ordered sequence of elements, achieving $O(\log n)$ average-time complexity. It behaves similarly to a balanced binary search tree but utilizes forward pointer layers (towers) determined by randomized geometric coin flips rather than strict tree rebalancing rotations.

---

## 1. Core Architecture & Multi-Level Pointers

A Skip List consists of multiple hierarchical singly linked lists:
- **Level 0 (Base Level):** Contains all elements in sorted ascending order.
- **Level $i$ ($i > 0$):** Acts as an "express lane" containing a subset of elements from Level $i-1$. An element in level $i$ has probability $p$ (commonly $p = 0.5$) of promoting to level $i+1$.

```
Level 3: [Head] ─────────────────────────────► [ 30 ] ───────────────────► [NIL]
                  │                                │
Level 2: [Head] ──────────────► [ 15 ] ────────► [ 30 ] ────────► [ 50 ] ─► [NIL]
                  │                │               │                │
Level 1: [Head] ──► [ 7 ] ─────► [ 15 ] ────────► [ 30 ] ─► [ 42 ] ─► [ 50 ] ─► [NIL]
                  │    │           │               │        │       │
Level 0: [Head] ──► [ 7 ] ─► [10]─► [ 15 ] ─► [21] ─► [ 30 ] ─► [ 42 ] ─► [ 50 ] ─► [NIL]
```

### Operations
- **`search(key)`**: Start at the highest level of `Head`. Traverse forward as long as `curr->forward[level]->key < target`. If the next key is $\ge$ target, drop down one level (`level--`). At level 0, if `curr->forward[0]->key == target`, the element is found.
- **`insert(key, value)`**: Track the `update[MAX_LEVEL]` array storing the rightmost node traversed at each level before dropping down. Generate a random height using geometric distribution ($P = 0.5$). Splice the new node into levels $0 \le i < \text{node\_level}$.
- **`delete(key)`**: Similar to search, find update pointers at each level and unlink the node from levels where it appears.

---

## 2. Complexity Analysis

| Operation | Average Time | Worst Case Time | Space Complexity |
| :--- | :---: | :---: | :---: |
| **Search** | $O(\log n)$ | $O(n)$ | $O(1)$ auxiliary |
| **Insert** | $O(\log n)$ | $O(n)$ | $O(\log n)$ expected node height |
| **Delete** | $O(\log n)$ | $O(n)$ | $O(1)$ auxiliary |
| **Total Memory** | $O(n)$ | $O(n \log n)$ | Average $n / (1 - p)$ forward pointers |

---

## 3. Language Implementation Highlights

- **C (`c/17_skip_list/`)**: Node structures with dynamic/flexible forward pointer arrays (`skiplist_node_t *forward[]`), pseudo-random level generator, and explicit sentinel head.
- **C++ (`cpp/17_skip_list/`)**: Modern templated `SkipList<Key, Value, Compare>` with modern random number generation (`std::mt19937`) and clean RAII memory management.
- **Safe Rust (`rust/src/ds_17_skip_list/`)**: High-performance, memory-safe index arena or vector-linked towers avoiding raw unsafe pointer dereferences.

