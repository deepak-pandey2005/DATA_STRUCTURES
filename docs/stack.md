# Stack

## 1. Overview
A **Stack** is a linear Abstract Data Type (ADT) that adheres strictly to the **LIFO (Last-In, First-Out)** principle: the most recently added item is the first one removed.

```
          Push (Insert)   |    ^  Pop (Remove)
                          V    |
                   +-------------+
                   |  Element 3  | <--- Top of Stack
                   +-------------+
                   |  Element 2  |
                   +-------------+
                   |  Element 1  |
                   +-------------+
```

---

## 2. Core Operations & Complexity

| Operation | Description | Time Complexity | Auxiliary Space |
| :--- | :--- | :--- | :--- |
| **`push(val)`** | Adds `val` to the top of the stack | $O(1)$ amortized (Array) / $O(1)$ (Linked List) | $O(1)$ |
| **`pop()`** | Removes and returns the top element | $O(1)$ | $O(1)$ |
| **`peek()` / `top()`** | Inspects the top element without removing | $O(1)$ | $O(1)$ |
| **`is_empty()`** | Checks whether size is zero | $O(1)$ | $O(1)$ |
| **`size()`** | Returns number of elements | $O(1)$ | $O(1)$ |

---

## 3. Underlying Implementations

1. **Array-Backed Stack**:
   - Built on top of a dynamic array.
   - Best performance, minimal memory overhead, optimal cache locality.
   - Occasional $O(N)$ resize cost on push (amortized $O(1)$).

2. **Linked-List-Backed Stack**:
   - Push and pop operate at the list's `head`.
   - Guaranteed $O(1)$ worst-case per operation with no reallocation bursts.
   - Extra pointer overhead per node.

---

## 4. Common Applications
- **Function Call Stack / Recursion**: CPU execution frames.
- **Expression Evaluation & Parsing**: Shunting-Yard algorithm (infix to postfix conversion, balanced parenthesis verification `()[]{}`).
- **Graph Traversal**: Depth-First Search (DFS).
- **Monotonic Stacks**: Next greater element problems in $O(N)$ time.
