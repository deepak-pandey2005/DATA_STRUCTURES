# Segment Tree

A **Segment Tree** is a tree data structure used for storing information about intervals, or segments. It allows answering range queries (e.g., range sum, range minimum/maximum) and performing point or range updates in logarithmic $O(\log n)$ time.

---

## 1. Core Mechanics & Architecture

For an array of $n$ elements, the segment tree is structured as a binary tree:
- **Root node:** Represents the query value for the entire array range $[0, n - 1]$.
- **Internal node:** For any node representing range $[L, R]$ with $M = \lfloor \frac{L + R}{2} \rfloor$:
  - Left child represents interval $[L, M]$.
  - Right child represents interval $[M + 1, R]$.
- **Leaf nodes:** Represent single-element intervals $[i, i]$ storing `arr[i]`.

### Flat Array Storage
Like a binary heap, a Segment Tree can be stored in a contiguous 1D array of size up to $4n$:
- Left Child of node $i$: $2i + 1$
- Right Child of node $i$: $2i + 2$

```
                   [0..3] (Sum: 16)
                  /                \
        [0..1] (Sum: 6)        [2..3] (Sum: 10)
        /            \         /             \
    [0] (1)        [1] (5)   [2] (3)        [3] (7)

Original Array: [1, 5, 3, 7]
```

---

## 2. Operations & Complexity Analysis

| Operation | Time Complexity | Auxiliary Space | Description |
| :--- | :--- | :--- | :--- |
| **Build** | $O(n)$ | $O(n)$ | Recursively builds tree bottom-up from input array. |
| **Point Update** | $O(\log n)$ | $O(\log n)$ stack | Updates a single leaf and bubbles adjustments up to the root. |
| **Range Query** | $O(\log n)$ | $O(\log n)$ stack | Merges at most 2 segment values per level of the tree. |

---

## 3. Multi-Language Implementations

- **C (`c/13_segment_tree/`)**: Array-backed segment tree with range sum query and point update.
- **C++ (`cpp/13_segment_tree/`)**: Templated `SegmentTree<T, MergeOp>` supporting custom associative combining operations (Sum, Min, Max, GCD).
- **Rust (`rust/src/ds_13_segment_tree/`)**: Safe Rust generic implementation supporting associative merge closures.
