# Queue

## 1. Overview
A **Queue** is an Abstract Data Type (ADT) that adheres to the **FIFO (First-In, First-Out)** principle: the earliest added element is the first one processed.

```
Enqueue (Insert) ---> [ E_n | ... | E_2 | E_1 ] ---> Dequeue (Remove)
(Rear / Back)                                         (Front / Head)
```

---

## 2. Key Variations & Mechanics

1. **Circular Ring Buffer (Array-based)**:
   - Maintains fixed or dynamic capacity with `head` and `tail` pointers that wrap around modulo `capacity`:
     ```c
     tail = (tail + 1) % capacity;
     head = (head + 1) % capacity;
     ```
   - Avoids $O(N)$ shifting required by a naive array dequeue.

2. **Linked-List Queue**:
   - Maintains `head` (for dequeue) and `tail` (for enqueue).
   - Zero reallocation latency.

---

## 3. Complexity Analysis

| Operation | Description | Time Complexity | Auxiliary Space |
| :--- | :--- | :--- | :--- |
| **`enqueue(val)`** | Appends `val` to the rear | $O(1)$ amortized (or guaranteed $O(1)$) | $O(1)$ |
| **`dequeue()`** | Removes and returns the front item | $O(1)$ | $O(1)$ |
| **`peek()` / `front()`** | Reads the front item | $O(1)$ | $O(1)$ |
| **`is_empty()`** | Checks whether queue is empty | $O(1)$ | $O(1)$ |
| **`size()`** | Number of active elements | $O(1)$ | $O(1)$ |

---

## 4. Real-World Applications
- **Breadth-First Search (BFS)**: Level-order traversal of graphs and trees.
- **Operating Systems**: Task schedulers, print spoolers, I/O request buffering.
- **Networking**: Packet buffers, message brokers (Kafka, RabbitMQ).
