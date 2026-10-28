# Disjoint Set Union (Union-Find / DSU)

The **Disjoint Set Union** (also called **Union-Find**) is a partition data structure that tracks elements split into several non-overlapping (disjoint) subsets.

---

## 1. Core Mechanics & Optimizations

DSU maintains a forest of trees where each tree represents a distinct set, and each node points to its parent. The root of each tree serves as the **canonical representative** of that set.

### 1. Path Compression (`find`)
During a `find(x)` operation, every node along the path from $x$ up to the root is updated to point directly to the root. This flattens the tree dynamically:

```
Before find(4):             After find(4):
      0                           0
      |                        / / \ \
      1                       1  2  3 4
      |
      2
      |
      3
      |
      4
```

### 2. Union by Rank / Size (`union`)
When merging two sets represented by roots $u$ and $v$:
- Always attach the root of the tree with **smaller rank (or size)** to become a child of the tree with **larger rank**.
- Prevents tree skewness, maintaining a logarithmic upper bound before path compression.

---

## 2. Operations & Complexity Analysis

| Operation | Time Complexity | Space Complexity | Description |
| :--- | :--- | :--- | :--- |
| **Make Set** | $O(1)$ | $O(n)$ total | Initialize element as its own parent with rank 0. |
| **Find** | $\alpha(n) \approx O(1)$ | $O(1)$ | Identify the canonical root of the set containing element $x$ with path compression. |
| **Union** | $\alpha(n) \approx O(1)$ | $O(1)$ | Merge two sets containing elements $x$ and $y$ using rank comparison. |
| **Connected** | $\alpha(n) \approx O(1)$ | $O(1)$ | Check if `find(x) == find(y)`. |

Here $\alpha(n)$ is the **Inverse Ackermann Function**, which grows so slowly that for any practical input size $n < 10^{80}$, $\alpha(n) \le 4$.

---

## 3. Applications

- **Kruskal's Algorithm:** Efficient Minimum Spanning Tree (MST) computation by preventing edge cycles.
- **Dynamic Graph Connectivity:** Checking whether two vertices are connected in an undirected graph under online edge additions.
- **Percolation & Clustering:** Grid clustering and connected component labeling in image processing.
