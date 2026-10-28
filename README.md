# 🚀 Data Structures & Algorithms (C11, Modern C++20, and Safe Rust)

[![C11](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![C++20](https://img.shields.io/badge/Language-C++20-00599C.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Rust 2021](https://img.shields.io/badge/Language-Rust%202021-DEA584.svg)](https://www.rust-lang.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A production-grade, modular reference repository implementing 12 foundational and advanced data structures uniformly across **C (C11)**, **Modern C++ (C++20)**, and **Safe Rust (2021 Edition)**.

Every data structure is accompanied by **visual architectural diagrams**, comprehensive markdown documentation, and passing test suites across all three languages.

---

## 📑 Table of Contents
1. [Visual Guide & Architecture Overview](#-visual-guide--architecture-overview)
2. [Complexity & Comparison Matrix](#-complexity--comparison-matrix)
3. [Repository Directory Layout](#-repository-directory-layout)
4. [Quick Start & Running Tests](#-quick-start--running-tests)

---

## 🎨 Visual Guide & Architecture Overview

### 1. Dynamic Array (`Vector`)
Contiguous heap memory with exponential dynamic growth ($2\times$) when capacity is reached.
```
Index:    [ 0 ] [ 1 ] [ 2 ] [ 3 ] [ 4 ] [ 5 ] [ 6 ] [ 7 ]
Memory:  | 10  | 20  | 30  | 40  | 50  |  -  |  -  |  -  |
         | <------- size = 5 --------> | <--- unused ---> |
         | <---------------- capacity = 8 --------------> |
```
- **Documentation:** [`docs/dynamic_array.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/dynamic_array.md)
- **Key Highlight:** $O(1)$ amortized append, contiguous cache locality.

---

### 2. Singly Linked List
Discrete heap nodes chained through a single forward `next` pointer.
```
 [ Head ]
    │
    ▼
┌───────┬──────┐     ┌───────┬──────┐     ┌───────┬──────┐
│ val:1 │ next ┼────►│ val:2 │ next ┼────►│ val:3 │ NULL │
└───────┴──────┘     └───────┴──────┘     └───────┴──────┘
                                                 ▲
                                                 │
                                              [ Tail ]
```
- **Documentation:** [`docs/singly_linked_list.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/singly_linked_list.md)
- **Key Highlight:** $O(1)$ prepend/append, non-contiguous allocations, iterative leak-free teardown.

---

### 3. Doubly Linked List
Bidirectional pointer links allowing $O(1)$ node removal from both ends and bidirectional iteration.
```
       [ Head ]                                             [ Tail ]
          │                                                    │
          ▼                                                    ▼
       ┌──────┬───────┬──────┐      ┌──────┬───────┬──────┐      ┌──────┬───────┬──────┐
NULL ◄─┼ prev │ val:1 │ next ┼─────►│ prev │ val:2 │ next ┼─────►│ prev │ val:3 │ next ┼─► NULL
       └──────┴───────┴──────┘◄─────┴──────┴───────┴──────┘◄─────┴──────┴───────┴──────┘
```
- **Documentation:** [`docs/doubly_linked_list.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/doubly_linked_list.md)
- **Key Highlight:** $O(1)$ push/pop at head and tail; cycle-safe Rust ownership via `Option<Box<Node>>`.

---

### 4. Stack (LIFO)
Last-In, First-Out sequence backed by a dynamic array.
```
  Push ───► | [ 40 ] | ◄── Top
            | [ 30 ] |
            | [ 20 ] |
  Pop  ◄─── | [ 10 ] |
            +--------+
```
- **Documentation:** [`docs/stack.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/stack.md)
- **Key Highlight:** $O(1)$ push, pop, and top inspection.

---

### 5. Queue (FIFO - Circular Ring Buffer)
First-In, First-Out circular buffer eliminating $O(n)$ shifts on dequeue.
```
                    Dequeue ◄── [Head: 2]
                                   │
      [0]       [1]       [2]      ▼[3]      [4]       [5]
   ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
   │   50    │    -    │   20    │   30    │   40    │    -    │
   └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
        ▲                                       ▲
        │                                       │
     Wrap-around ─── Enqueue ──► [Tail: 0]
```
- **Documentation:** [`docs/queue.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/queue.md)
- **Key Highlight:** $O(1)$ enqueue and dequeue; modulo-based pointer wrapping.

---

### 6. Binary Search Tree (BST)
Ordered tree satisfying the invariant: $\text{left} < \text{node} < \text{right}$.
```
                 [ 50 ]
                /      \
            [ 30 ]    [ 70 ]
           /     \    /     \
        [ 20 ] [ 40 ][ 60 ] [ 80 ]
```
- **Documentation:** [`docs/binary_search_tree.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/binary_search_tree.md)
- **Key Highlight:** 3-case deletion (leaf, 1 child, 2 children via in-order successor); sorted in-order traversal.

---

### 7. Binary Heap & Priority Queue
Complete binary tree stored compactly inside a flat dynamic array.
```
Tree View:                    Array Mapping:
          [0] 30               Index:  0   1   2   3   4   5
         /      \              Value: [30, 25, 20, 10, 15, 8]
      [1] 25    [2] 20         Parent(i) = (i - 1) / 2
     /     \     /             Left(i)   = 2i + 1
  [3] 10 [4] 15 [5] 8          Right(i)  = 2i + 2
```
- **Documentation:** [`docs/heap.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/heap.md)
- **Key Highlight:** $O(1)$ peek, $O(\log n)$ push/pop with sift-up/sift-down, $O(n)$ bottom-up heapify.

---

### 8. Hash Table (Separate Chaining)
Associative key-value map with 64-bit FNV-1a hashing and linked collision chains.
```
  Key: "apple" ──► [ FNV-1a Hash ] ──► Hash % 4 = Bucket [1]

  Buckets:
   [ 0 ] ──► NULL
   [ 1 ] ──► [ "apple": 10 ] ──► [ "banana": 25 ] ──► NULL   (Collision resolved via chain)
   [ 2 ] ──► NULL
   [ 3 ] ──► [ "cherry": 90 ] ──► NULL
```
- **Documentation:** [`docs/hash_table.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/hash_table.md)
- **Key Highlight:** Amortized $O(1)$ lookup/insert; automated rehashing when load factor $\alpha \ge 0.75$.

---

### 9. Trie (Prefix Tree)
26-way alphabet retrieval tree for fast dictionary and prefix searches.
```
               (Root)
              /      \
            'b'      'c'
            /          \
          'a'          'a'
          / \            \
        't'  'n'         't' (is_end: true)  -> "cat"
   (is_end)   \
   -> "bat"   'd' (is_end: true) -> "band"
```
- **Documentation:** [`docs/trie.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/trie.md)
- **Key Highlight:** $O(L)$ word lookup and prefix matching independent of total dictionary size $n$.

---

### 10. Disjoint Set Union (Union-Find / DSU)
Forest of trees with path compression and union by rank.
```
Path Compression (Find):              Union by Rank (Merge):
       0                      0                0               0
       |                    / / \             / \             /|\
       1    ── find(3) ──► 1  2  3           1   2   +   3 ──►1 2 3
       |                                   (Rank 1)   (Rank 0)
       2
       |
       3
```
- **Documentation:** [`docs/dsu.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/dsu.md)
- **Key Highlight:** Near $O(1)$ operations ($\alpha(n) \le 4$ Inverse Ackermann); cycle detection in graphs.

---

### 11. AVL Tree (Self-Balancing BST)
Guarantees $O(\log n)$ worst-case time by enforcing $\text{Balance Factor} \in \{-1, 0, 1\}$.
```
   Left-Left Overload (BF = +2):                 After Right Rotation:
              Z                                            Y
             / \                                         /   \
            Y   T3       ── Right Rotation ──►          X     Z
           / \                                         / \   / \
          X   T2                                      T0 T1 T2 T3
         / \
        T0  T1
```
- **Documentation:** [`docs/avl_tree.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/avl_tree.md)
- **Key Highlight:** Strict height tracking, 4 rotation rebalancing cases (LL, RR, LR, RL).

---

### 12. Graph (Adjacency List + BFS / DFS)
Space-efficient graph representation supporting directed/undirected edges and standard traversals.
```
Graph Topology:                      Adjacency List:
     (0) ────── (1)                  0: [1] ──► [3]
      │        ╱ │                   1: [0] ──► [2]
      │      ╱   │                   2: [1] ──► [3] ──► [4]
      │    ╱     │                   3: [0] ──► [2]
     (3) ────── (2) ────── (4)       4: [2]
```
- **Documentation:** [`docs/graph.md`](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/graph.md)
- **Key Highlight:** $O(V + E)$ space, Breadth-First Search (shortest unweighted paths), and Depth-First Search.

---

## 📊 Complexity & Comparison Matrix

| Data Structure | Access | Search | Insert | Delete | Space Complexity |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Dynamic Array (Vector)** | $O(1)$ | $O(n)$ | $O(1)$ amortized | $O(n)$ | $O(n)$ |
| **Singly Linked List** | $O(n)$ | $O(n)$ | $O(1)$ | $O(1)$ (known node) | $O(n)$ |
| **Doubly Linked List** | $O(n)$ | $O(n)$ | $O(1)$ | $O(1)$ (known node) | $O(n)$ |
| **Stack** | $O(n)$ | $O(n)$ | $O(1)$ (push) | $O(1)$ (pop) | $O(n)$ |
| **Queue (Ring Buffer)** | $O(n)$ | $O(n)$ | $O(1)$ (enqueue) | $O(1)$ (dequeue) | $O(n)$ |
| **Binary Search Tree** | $O(\log n)$ avg | $O(\log n)$ avg | $O(\log n)$ avg | $O(\log n)$ avg | $O(n)$ |
| **Binary Heap (Priority Queue)**| $O(1)$ (peek) | $O(n)$ | $O(\log n)$ | $O(\log n)$ | $O(n)$ |
| **Hash Table** | N/A | $O(1)$ amortized | $O(1)$ amortized | $O(1)$ amortized | $O(n)$ |
| **Trie (Prefix Tree)** | N/A | $O(L)$ | $O(L)$ | $O(L)$ | $O(N \cdot \Sigma)$ |
| **Disjoint Set Union (DSU)** | N/A | $\alpha(n)$ | $\alpha(n)$ | N/A | $O(n)$ |
| **AVL Tree** | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(n)$ |
| **Graph (Adjacency List)** | N/A | $O(V + E)$ | $O(1)$ | $O(\text{deg}(v))$ | $O(V + E)$ |

---

## 📁 Repository Directory Layout

Each language implementation is neatly separated into dedicated subdirectories per data structure:

```text
DATA_STRUCTURES/
├── Makefile                          # Unified root test runner across C, C++, and Rust
├── README.md                         # Visual architectural overview and documentation
├── docs/                             # Conceptual architecture and mathematical analysis
│   ├── dynamic_array.md
│   ├── singly_linked_list.md
│   ├── doubly_linked_list.md
│   ├── stack.md
│   ├── queue.md
│   ├── binary_search_tree.md
│   ├── heap.md
│   ├── hash_table.md
│   ├── trie.md
│   ├── dsu.md
│   ├── avl_tree.md
│   └── graph.md
├── c/                                # C11 implementations (isolated per data structure)
│   ├── Makefile
│   ├── common/include/               # Shared status codes (common.h)
│   ├── vector/                       # include/vector.h, src/vector.c
│   ├── singly_linked_list/
│   ├── doubly_linked_list/
│   ├── stack/
│   ├── queue/
│   ├── bst/
│   ├── heap/
│   ├── hash_table/
│   ├── trie/
│   ├── dsu/
│   ├── avl_tree/
│   ├── graph/
│   └── tests/test_runner.c           # Comprehensive C test suite
├── cpp/                              # Modern C++20 templated implementations
│   ├── Makefile
│   ├── vector/                       # include/Vector.hpp, src/Vector.cpp
│   ├── singly_linked_list/
│   ├── doubly_linked_list/
│   ├── stack/
│   ├── queue/
│   ├── bst/
│   ├── heap/
│   ├── hash_table/
│   ├── trie/
│   ├── dsu/
│   ├── avl_tree/
│   ├── graph/
│   └── tests/test_main.cpp           # Modern C++ test suite
└── rust/                             # Safe Rust modular crate
    ├── Cargo.toml
    └── src/
        ├── lib.rs                    # Root crate exports & unit tests
        ├── vector/mod.rs
        ├── singly_linked_list/mod.rs
        ├── doubly_linked_list/mod.rs
        ├── stack/mod.rs
        ├── queue/mod.rs
        ├── bst/mod.rs
        ├── heap/mod.rs
        ├── hash_table/mod.rs
        ├── trie/mod.rs
        ├── dsu/mod.rs
        ├── avl_tree/mod.rs
        └── graph/mod.rs
```

---

## ⚡ Quick Start & Running Tests

### Prerequisites
- **GCC / Clang** (C11 support)
- **G++ / Clang++** (C++20 support)
- **Rust & Cargo** (1.70+)
- **Make**

### Run All Tests Across All 3 Languages in One Command:
```bash
make test
```

### Run Tests for Individual Languages:
```bash
# Run C test suite (12 tests)
cd c && make test

# Run C++ test suite (12 tests)
cd cpp && make test

# Run Rust unit test suite (12 tests)
cargo test --manifest-path rust/Cargo.toml
```
