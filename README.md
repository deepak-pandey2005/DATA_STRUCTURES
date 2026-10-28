# Data Structures and Algorithms in C, C++, and Rust

A comprehensive, production-grade repository implementing foundational and advanced data structures and algorithms across **C (C11)**, **Modern C++ (C++20)**, and **Rust (2021 Edition)**, accompanied by conceptual documentation and complexity analysis.

---

## 📚 Architectural Guides & Documentation

In-depth conceptual guides with memory layouts, time/space complexity, diagrams, and language-specific design considerations:

- [Dynamic Array (Vector)](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/dynamic_array.md) - Contiguous heap allocation, amortized $O(1)$ growth, cache locality.
- [Singly Linked List](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/singly_linked_list.md) - Discrete node chaining, $O(1)$ prepend/append, iterative cleanup.
- [Doubly Linked List](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/doubly_linked_list.md) - Bidirectional traversal, $O(1)$ node removal, cycle-safe Rust mechanics.
- [Stack](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/stack.md) - LIFO mechanics, vector backing, expression evaluation.
- [Queue](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/queue.md) - FIFO mechanics, dynamic circular ring-buffer wrapping.
- [Binary Search Tree (BST)](file:///home/deepak/OPENSOURCE/DATA_STRUCTURES/docs/binary_search_tree.md) - Hierarchical search invariant, 3-case node deletion, in-order sorted traversal.

---

## 📁 Repository Organization

```
DATA_STRUCTURES/
├── Makefile                    # Unified root test runner across all languages
├── README.md                   # Repository overview, architecture, & roadmap
├── docs/                       # Comprehensive guides and complexity analysis
│   ├── dynamic_array.md
│   ├── singly_linked_list.md
│   ├── doubly_linked_list.md
│   ├── stack.md
│   ├── queue.md
│   └── binary_search_tree.md
├── c/                          # C11 implementation (manual memory, defensive pointers)
│   ├── Makefile
│   ├── include/                # Header files (vector.h, bst.h, etc.)
│   ├── src/                    # Source implementations
│   └── tests/                  # Test suite runner (test_runner.c)
├── cpp/                        # Modern C++20 (Templates, RAII, Move Semantics, Iterators)
│   ├── Makefile
│   ├── include/                # Header declarations (Vector.hpp, BST.hpp, etc.)
│   ├── src/                    # Compiled implementation translation units
│   └── tests/                  # Test suite (test_main.cpp)
└── rust/                       # Safe Rust (Ownership, Option/Result, Traits, Drop)
    ├── Cargo.toml
    └── src/                    # Library modules & unit test suite
```

---

## ⚡ Quick Start & Running Tests

### 1. Run All Test Suites (C, C++, and Rust)
From the repository root:
```bash
make test
```

### 2. Language-Specific Commands

#### C (GCC / Clang + Make)
```bash
make -C c test
```

#### Modern C++ (G++ / Clang++ + Make)
```bash
make -C cpp test
```

#### Rust (Cargo)
```bash
cargo test --manifest-path rust/Cargo.toml
```

---

## 🔍 Language Paradigm Comparison

| Feature | C (C11) | C++ (C++20) | Rust (2021 Edition) |
| :--- | :--- | :--- | :--- |
| **Generics** | `void*` or typed structs | Templates (`template <typename T>`) | Generics (`<T>`) + Trait bounds |
| **Memory Management**| Manual (`malloc` / `realloc` / `free`) | Automatic RAII (Destructors, Rule of 5) | Ownership & Lifetimes (Automatic `Drop`) |
| **Error Handling** | Enum status codes (`ds_status_t`) | Standard Exceptions (`std::out_of_range`) | `Option<T>` & `Result<T, E>` |
| **Safety Guarantees**| Manual bounds assertions | Bounds-checked `.at()` + raw indexing | Compiler-enforced memory safety |
| **Iterators** | Manual pointer/index traversal | Forward/Bidirectional C++ Iterators | Standard `Iterator` trait |

---

## 🗺️ Roadmap & Upcoming Phases

- **Phase 1 (Completed)**: Core foundational data structures (Dynamic Array, Singly Linked List, Doubly Linked List, Stack, Queue, Binary Search Tree).
- **Phase 2 (Advanced Trees & Heaps)**: AVL Trees, Red-Black Trees, Binary Min/Max Heaps, Priority Queues.
- **Phase 3 (Hash Tables & Strings)**: Hash Map (Open Addressing & Chaining), Trie (Prefix Tree), Bloom Filter.
- **Phase 4 (Sorting & Searching Algorithms)**: Quicksort, Mergesort, Heapsort, Counting Sort, Binary Search, Quickselect.
- **Phase 5 (Graph Algorithms)**: BFS, DFS, Dijkstra's Shortest Path, Bellman-Ford, Kruskal's / Prim's MST, Topological Sort.
- **Phase 6 (Advanced Dynamic Programming)**: Knapsack, LCS, LIS, Matrix Chain Multiplication.
