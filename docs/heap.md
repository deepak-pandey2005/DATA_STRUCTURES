# Binary Heap & Priority Queue

A **Binary Heap** is a complete binary tree that satisfies the **heap invariant**. It is the standard underlying engine for implementing an efficient **Priority Queue**.

---

## 1. Core Mechanics & Invariants

A Binary Heap is maintained as a **complete binary tree** (all levels filled except possibly the bottom, filled from left to right), allowing it to be stored compactly inside a contiguous dynamic array without node pointers.

### Invariants
1. **Shape Property:** The tree is completely filled on all levels except possibly the lowest, which is populated strictly from left to right.
2. **Heap Invariant:**
   - **Min-Heap:** For every node $i$ other than the root, $\text{parent}(i) \le \text{node}(i)$. The minimum element is always at the root ($index = 0$).
   - **Max-Heap:** For every node $i$ other than the root, $\text{parent}(i) \ge \text{node}(i)$. The maximum element is always at the root ($index = 0$).

---

## 2. Array Representation & Indexing

Given an element at 0-based index $i$:
- **Parent:** $\lfloor \frac{i - 1}{2} \rfloor$
- **Left Child:** $2i + 1$
- **Right Child:** $2i + 2$

```
          [0] 10
         /      \
      [1] 15    [2] 20
     /     \
  [3] 17  [4] 25

Array: [10, 15, 20, 17, 25]
```

---

## 3. Operations & Complexity Analysis

| Operation | Time Complexity | Auxiliary Space | Description |
| :--- | :--- | :--- | :--- |
| **Peek (Min/Max)** | $O(1)$ | $O(1)$ | Read element at index `0`. |
| **Push / Insert** | $O(\log n)$ | $O(1)$ | Append to end, then sift up (`sift_up`). |
| **Pop / Extract** | $O(\log n)$ | $O(1)$ | Swap root with last element, pop, then sift down root (`sift_down`). |
| **Heapify (Build Heap)** | $O(n)$ | $O(1)$ | Run bottom-up sift-down starting from last non-leaf parent $(\lfloor n/2 \rfloor - 1)$. |

---

## 4. Multi-Language Implementations

- **C (`c/heap/`)**: Generic comparator-based implementation using raw bytes and function pointers (`heap_compare_fn`). Supports both min-heap and max-heap.
- **C++ (`cpp/heap/`)**: Type-safe generic template `BinaryHeap<T, Compare>` supporting custom functors like `std::less<T>` (max-heap) or `std::greater<T>` (min-heap).
- **Rust (`rust/src/heap/`)**: Generic `BinaryHeap<T>` using `Ord` and custom predicates with safe vector backing.
