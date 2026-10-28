# Binary Search Tree (BST)

## 1. Overview
A **Binary Search Tree** is a rooted, hierarchical binary tree structure where each node satisfies the **BST Invariant**:
- Every node in the **left subtree** has a key strictly less than (or $\le$) the node's key.
- Every node in the **right subtree** has a key strictly greater than (or $\ge$) the node's key.
- Both left and right subtrees must themselves be binary search trees.

```
            (20)
           /    \
        (10)    (30)
        /  \       \
      (5)  (15)    (40)
```

---

## 2. Core Operations

### 2.1 Search
Recursively or iteratively compare target key $k$ with `current->key`:
- If $k == \text{current}$, found!
- If $k < \text{current}$, descend left.
- If $k > \text{current}$, descend right.

### 2.2 Insertion
Traverse according to BST ordering until finding an empty leaf slot (`NULL`), then link the new node.

### 2.3 Deletion
Handling three cases:
1. **Node has no children (Leaf)**: Disconnect and free node.
2. **Node has one child**: Splice the child directly to the node's parent.
3. **Node has two children**: Find the **In-Order Successor** (smallest key in the right subtree) or In-Order Predecessor (largest key in left subtree), copy its value into the current node, then recursively delete that successor.

### 2.4 Tree Traversals
- **In-Order (Left, Root, Right)**: Yields elements in strictly sorted ascending order.
- **Pre-Order (Root, Left, Right)**: Useful for serialization and cloning tree topologies.
- **Post-Order (Left, Right, Root)**: Useful for bottom-up computation (e.g. subtree sizes, freeing memory/destructor).
- **Level-Order (BFS)**: Breadth-first traversal layer by layer using a queue.

---

## 3. Complexity Analysis

| Operation | Average Case | Worst Case (Degenerate / Skewed) | Space (Recursion Depth) |
| :--- | :--- | :--- | :--- |
| **Search** | $O(\log N)$ | $O(N)$ (linked-list shape) | $O(h)$ where $h = \text{height}$ |
| **Insert** | $O(\log N)$ | $O(N)$ | $O(h)$ |
| **Delete** | $O(\log N)$ | $O(N)$ | $O(h)$ |
| **Traversals**| $O(N)$ | $O(N)$ | $O(h)$ auxiliary stack |

> **Note**: To prevent the worst-case $O(N)$ degradation when inserting sorted data, self-balancing variants such as **AVL Trees** and **Red-Black Trees** maintain $h \le c \cdot \log N$ via tree rotations.

---

## 4. Language Implementation Strategies
- **C**: Explicit recursive pointers `struct BSTNode* left, *right`. Custom recursive destructor to prevent memory leaks.
- **C++**: Generic `BinarySearchTree<T>` with RAII lifecycle, copy/move constructors, custom lambda or callback traversal visitor, and deep cloning.
- **Rust**: Using `Option<Box<BSTNode<T>>>`:
  ```rust
  pub struct BSTNode<T> {
      pub value: T,
      pub left: Option<Box<BSTNode<T>>>,
      pub right: Option<Box<BSTNode<T>>>,
  }
  ```
