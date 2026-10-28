# Singly Linked List

## 1. Overview
A **Singly Linked List** is a linear data structure composed of discrete nodes stored at non-contiguous memory locations. Each node stores two items:
1. **`data`**: The payload value.
2. **`next`**: A pointer / reference to the subsequent node in the sequence (or `NULL` / `None` at the tail).

```
[ Head: 10 | * ] ---> [ 20 | * ] ---> [ 30 | * ] ---> NULL
```

---

## 2. Key Mechanics & Invariants
- **Head Pointer**: Tracks the entry point to the list. If `head == NULL`, the list is empty.
- **Node Allocation**: Each node is allocated independently on the heap.
- **Splicing & Linking**: Insertion or deletion at the head takes $O(1)$ operations with zero shifting of remaining elements.
- **No Random Access**: To access the $k$-th element, one must traverse $k$ pointers starting from `head`.

---

## 3. Complexity Analysis

| Operation | Time Complexity (Average) | Time Complexity (Worst) | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Access (k-th element)** | $O(N)$ | $O(N)$ | $O(1)$ |
| **Search (value)** | $O(N)$ | $O(N)$ | $O(1)$ |
| **Insert at Head (Push Front)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Insert at Tail (with tail pointer)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Insert After Node** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Delete at Head (Pop Front)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Delete Value (Unsorted)** | $O(N)$ | $O(N)$ | $O(1)$ |
| **Reverse In-Place** | $O(N)$ | $O(N)$ | $O(1)$ |

---

## 4. Trade-offs vs Dynamic Arrays
- **Pros**:
  - $O(1)$ prepend and append without reallocation hiccups or capacity over-allocation.
  - Efficient splitting and splicing.
- **Cons**:
  - **Poor cache locality**: Nodes are scattered across the heap, causing CPU cache misses on pointer traversal.
  - **Memory Overhead**: Every payload incurs pointer storage overhead (8 bytes on 64-bit systems per node).

---

## 5. Idiomatic Language Implementations

- **C**: Manual allocation with `malloc(sizeof(Node))` and pointer chasing. Requires defensive checks to avoid memory leaks during deletions.
- **C++**: Encapsulated within a class with custom iterators (`iterator`, `const_iterator`), RAII cleanup in destructor, and copy/move constructors.
- **Rust**: Naturally represented recursively as:
  ```rust
  pub struct List<T> {
      head: Option<Box<Node<T>>>,
  }
  struct Node<T> {
      val: T,
      next: Option<Box<Node<T>>>,
  }
  ```
  Iterative `Drop` implementation prevents call-stack overflow on deep lists.
