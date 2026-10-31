# Fenwick Tree (Binary Indexed Tree / BIT)

A **Fenwick Tree**, or **Binary Indexed Tree (BIT)**, is an exceptionally compact and efficient data structure that can update elements and calculate prefix sums in a table of numbers in $O(\log n)$ time.

---

## 1. Core Mechanics: Lowest Set Bit (LSB / `x & (-x)`)

The Fenwick tree represents intervals using the binary representation of indices (using 1-based indexing for convenience):
- Each index $i$ stores the sum of elements in the interval $(i - \text{LSB}(i), i]$, where:
  $$\text{LSB}(i) = i \ \& \ (-i)$$

```
Index (binary)   LSB   Interval Covered
1     (0001)     1     [1..1]
2     (0010)     2     [1..2]
3     (0011)     1     [3..3]
4     (0100)     4     [1..4]
5     (0101)     1     [5..5]
6     (0110)     2     [5..6]
7     (0111)     1     [7..7]
8     (1000)     8     [1..8]
```

### Tree Visual Representation
```
                    [ 8 ] (covers 1..8)
                   /  |  \
             [ 4 ]  [ 6 ] [ 7 ]
            /  |     |
       [ 2 ] [ 3 ]  [ 5 ]
        |
       [ 1 ]
```

---

## 2. Operations & Complexity Analysis

| Operation | Time Complexity | Auxiliary Space | Description |
| :--- | :--- | :--- | :--- |
| **Prefix Sum (`query(i)`)** | $O(\log n)$ | $O(1)$ | Decrement $i \leftarrow i - (i \ \& \ -i)$ while accumulating sums. |
| **Point Update (`add(i, delta)`)** | $O(\log n)$ | $O(1)$ | Increment $i \leftarrow i + (i \ \& \ -i)$ while updating tree buckets. |
| **Range Query (`range_sum(L, R)`)** | $O(\log n)$ | $O(1)$ | Calculated as $\text{query}(R) - \text{query}(L - 1)$. |

---

## 3. Comparison with Segment Tree

| Feature | Fenwick Tree (BIT) | Segment Tree |
| :--- | :--- | :--- |
| **Space Required** | Exactly $n$ words ($O(n)$) | Up to $4n$ words ($O(n)$) |
| **Implementation Complexity** | ~20 lines of code, minimal overhead | ~60+ lines of recursive logic |
| **Range Types Supported** | Invertible functions (Sum, XOR, etc.) | Arbitrary associative functions (Min, Max, GCD) |
