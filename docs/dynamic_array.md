# Dynamic Array (Vector)

## 1. Overview
A **Dynamic Array** (often called `std::vector` in C++, `Vec` in Rust, or `ArrayList` in Java) is a contiguous, random-access data structure that can grow and shrink in capacity dynamically at runtime.

Unlike static arrays where the size must be known at compile time or fixed at allocation, dynamic arrays manage an underlying heap-allocated block of memory and resize it automatically when capacity is exhausted.

---

## 2. Memory Layout & Mechanics
A dynamic array is typically represented by three fields:
1. **`data`**: Pointer to the contiguous heap buffer storing elements.
2. **`size` / `len`**: Number of currently occupied elements.
3. **`capacity`**: Total allocated element slots currently available.

```
Buffer: [ 10 | 20 | 30 | 40 |    |    |    |    ]
         ^                   ^                   ^
       Index 0             size = 4        capacity = 8
```

### Growth Strategy (Amortized Analysis)
- When `size == capacity`, inserting a new element requires reallocation:
  1. Allocate a new buffer of `capacity * 2` (or factor `1.5x`).
  2. Copy/move all existing elements from old buffer to new buffer.
  3. Deallocate the old buffer.
  4. Point `data` to the new buffer.
- **Why double?** Doubling ensures that a sequence of $N$ insertions takes $O(N)$ total time, yielding **$O(1)$ amortized time** per push. If the capacity only increased by a constant amount (e.g., $+10$), $N$ insertions would take $O(N^2)$ time.

---

## 3. Complexity Analysis

| Operation | Time Complexity (Average) | Time Complexity (Worst) | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Access (by index)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Search (unsorted)** | $O(N)$ | $O(N)$ | $O(1)$ |
| **Insert at End (Push)**| $O(1)$ amortized | $O(N)$ (during resize) | $O(1)$ auxiliary |
| **Insert at Middle/Start** | $O(N)$ | $O(N)$ | $O(1)$ |
| **Delete at End (Pop)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Delete at Middle/Start** | $O(N)$ | $O(N)$ | $O(1)$ |

---

## 4. Cache Locality & Hardware Efficiency
Because elements are stored strictly contiguously in physical RAM:
- Sequential traversals experience maximum **L1/L2 cache hit rates**.
- Modern CPU prefetchers can load consecutive cache lines ahead of iteration.
- Dynamic arrays virtually always outperform linked lists in real-world benchmarks, even for some insertions/deletions, due to memory hierarchy effects.

---

## 5. Language Implementation Differences

| Feature | C | C++ (`Vector<T>`) | Rust (`Vec<T>`) |
| :--- | :--- | :--- | :--- |
| **Memory Allocation** | `malloc` / `realloc` / `free` | `new[]` / `delete[]` or custom allocator | `alloc::alloc` / Global Allocator |
| **Generics** | `void*` or type-erased byte buffers | C++ Templates (`template <typename T>`) | Rust Generics (`Vec<T>`) |
| **Destruction** | Explicit manual cleanup (`vector_free`) | RAII (automatic destructor invocation) | Drop trait (automatic memory deallocation) |
| **Safety** | Manual pointer checks & bound assertions | Exceptions / optional unchecked access | Checked `get` returns `Option<&T>`, bounds panic |
