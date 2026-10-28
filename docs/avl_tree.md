# AVL Tree (Self-Balancing Binary Search Tree)

An **AVL Tree** (named after inventors **A**delson-**V**elsky and **L**andis) is a self-balancing binary search tree. It maintains a strict balance invariant to guarantee that search, insertion, and deletion operations execute in strictly $O(\log n)$ worst-case time.

---

## 1. The Balance Invariant & Balance Factor

For every node $x$ in an AVL tree:
$$\text{Balance Factor}(x) = \text{height}(\text{left}(x)) - \text{height}(\text{right}(x))$$

### Strict AVL Invariant
$$\text{Balance Factor}(x) \in \{-1, 0, +1\}$$

If an insertion or deletion causes $\text{Balance Factor}(x) \notin \{-1, 0, +1\}$, the subtree rooted at $x$ is rebalanced using tree rotations.

---

## 2. Tree Rotations (4 Scenarios)

### Scenario 1: Left-Left (LL) Overload
- $\text{BF}(x) > 1$ and $\text{BF}(\text{left}(x)) \ge 0$.
- **Fix:** Perform a single **Right Rotation** around $x$.

### Scenario 2: Right-Right (RR) Overload
- $\text{BF}(x) < -1$ and $\text{BF}(\text{right}(x)) \le 0$.
- **Fix:** Perform a single **Left Rotation** around $x$.

### Scenario 3: Left-Right (LR) Overload
- $\text{BF}(x) > 1$ and $\text{BF}(\text{left}(x)) < 0$.
- **Fix:** Perform a **Left Rotation** on $\text{left}(x)$, followed by a **Right Rotation** on $x$.

### Scenario 4: Right-Left (RL) Overload
- $\text{BF}(x) < -1$ and $\text{BF}(\text{right}(x)) > 0$.
- **Fix:** Perform a **Right Rotation** on $\text{right}(x)$, followed by a **Left Rotation** on $x$.

```
     Right Rotation (around Z):             Left Rotation (around X):
           Z                                      X
          / \                                    / \
         Y   T3      -- Right Rotation -->      T1   Y
        / \          <-- Left Rotation --           / \
       X   T2                                      T2  Z
      / \                                             / \
     T0  T1                                          T3  T4
```

---

## 3. Operations & Complexity Analysis

| Operation | Average Case | Worst Case | Space Complexity | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Search** | $O(\log n)$ | $O(\log n)$ | $O(1)$ auxiliary | Standard BST traversal invariant. |
| **Insert** | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ call stack | BST insert, backtrack updating node heights, rotate if BF $\notin \{-1, 0, 1\}$. |
| **Delete** | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ call stack | 3-case BST deletion, backtrack updating node heights, rotate if unbalanced. |
| **Height** | $O(1)$ | $O(1)$ | $O(1)$ | Cached per node. |

---

## 4. Multi-Language Implementations

- **C (`c/avl_tree/`)**: Pointer-based node tree with cached `int height`, explicit rotations, in-order traversal, and recursive memory release.
- **C++ (`cpp/avl_tree/`)**: Templated `AVLTree<T>` with copy constructors, move semantics, and RAII node destruction.
- **Rust (`rust/src/avl_tree/`)**: Safe Rust implementation using `Option<Box<AvlNode<T>>>` with height caching and rotation mechanics.

