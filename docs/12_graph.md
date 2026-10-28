# Graph (Adjacency List with BFS & DFS)

A **Graph** is a non-linear data structure consisting of a set of **vertices** (or nodes) $V$ and a collection of **edges** $E$ connecting pairs of vertices.

---

## 1. Representation: Adjacency List

An **Adjacency List** represents a graph as an array (or map) of lists. For each vertex $u$, a list stores all adjacent destination vertices $v$ (and optionally edge weights $w$):

```
Graph:
 (0) ------ (1)
  |        / |
  |      /   |
  |    /     |
 (2) ------ (3)

Adjacency List:
0: -> [1] -> [2]
1: -> [0] -> [2] -> [3]
2: -> [0] -> [1] -> [3]
3: -> [1] -> [2]
```

### Advantages
- Space efficient for sparse graphs: $O(V + E)$ space vs. $O(V^2)$ for an Adjacency Matrix.
- Iterating over neighbors of a vertex takes $O(\text{deg}(v))$ time.

---

## 2. Core Graph Traversals

### Breadth-First Search (BFS)
- Explores vertices level-by-level using a **Queue** (FIFO).
- Guaranteed to find the **shortest path** in terms of number of edges on unweighted graphs.
- **Time Complexity:** $O(V + E)$
- **Space Complexity:** $O(V)$ for queue and visited set.

### Depth-First Search (DFS)
- Explores as deep as possible along each branch before backtracking using a **Stack** or recursion.
- Used for topological sorting, cycle detection, strongly connected components (SCC), and maze solving.
- **Time Complexity:** $O(V + E)$
- **Space Complexity:** $O(V)$ call stack and visited tracker.

---

## 3. Complexity Summary

| Traversal / Operation | Time Complexity | Space Complexity | Description |
| :--- | :--- | :--- | :--- |
| **Add Vertex** | $O(1)$ amortized | $O(1)$ | Append new neighbor list. |
| **Add Edge** | $O(1)$ | $O(1)$ | Append destination to source neighbor list. |
| **BFS Traversal** | $O(V + E)$ | $O(V)$ | Level-order queue search. |
| **DFS Traversal** | $O(V + E)$ | $O(V)$ | Backtracking depth search. |

