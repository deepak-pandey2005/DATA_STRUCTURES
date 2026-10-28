# Doubly Linked List

## 1. Overview
A **Doubly Linked List** extends the singly linked list by equipping each node with two directional pointers:
1. **`next`**: Reference to the successor node.
2. **`prev`**: Reference to the predecessor node.

```
NULL <--- [ prev | 10 | next ] <===> [ prev | 20 | next ] <===> [ prev | 30 | next ] ---> NULL
```

With both `head` and `tail` pointers maintained by the container, elements can be traversed forward and backward, and removal of an arbitrary node given only a pointer to it is $O(1)$.

---

## 2. Key Mechanics
- **Sentinel (Dummy) Nodes vs Direct Pointers**:
  - Direct pointers require checking edge cases (`head == NULL`, removing single element, modifying head/tail).
  - Sentinel nodes (`dummy_head`, `dummy_tail`) eliminate special edge-case branch logic by ensuring internal nodes always have valid `prev` and `next`.
- **Bidirectional Splicing**: Removing node $N$:
  ```c
  N->prev->next = N->next;
  N->next->prev = N->prev;
  ```

---

## 3. Complexity Analysis

| Operation | Time Complexity | Auxiliary Space |
| :--- | :--- | :--- |
| **Push Front / Pop Front** | $O(1)$ | $O(1)$ |
| **Push Back / Pop Back** | $O(1)$ | $O(1)$ |
| **Delete Given Node Pointer** | $O(1)$ | $O(1)$ |
| **Insert Before / After Given Node** | $O(1)$ | $O(1)$ |
| **Access / Search by Index** | $O(N)$ (can optimize to $N/2$ from closer end) | $O(1)$ |

---

## 4. Real-World Applications
- **LRU Cache (Least Recently Used)**: Combined with a Hash Map, a Doubly Linked List allows $O(1)$ updates to promote accessed items to the front and evict cold items from the tail.
- **Undo / Redo Buffers**: Step forward and backward through operational history.
- **OS Process / Thread Schedulers**: Ready queues where arbitrary threads may sleep or wake up.

---

## 5. Language Implementation Notes
- **C**: Direct pointer manipulation (`prev`, `next`).
- **C++**: Doubly-linked list with bidirectional iterators (`std::bidirectional_iterator_tag`).
- **Rust**: Safe doubly-linked lists with standard ownership are notoriously challenging because a node has two owners (`prev` and `next`). Standard approaches use:
  1. `Rc<RefCell<Node<T>>>` (interior mutability with reference counting), or
  2. Raw pointers (`*mut Node<T>`) inside an encapsulated `unsafe` module with safe public API (standard `std::collections::LinkedList` approach), or
  3. Arena / index-based vectors (`slotmap` or index handles).
