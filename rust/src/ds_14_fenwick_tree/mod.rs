//! Fenwick Tree (Binary Indexed Tree) in safe Rust.
//!
//! Provides O(log n) prefix sum queries and point updates.

pub struct FenwickTree {
    tree: Vec<i64>,
    size: usize,
}

impl FenwickTree {
    /// Creates an empty Fenwick tree of size n.
    pub fn new(size: usize) -> Self {
        Self {
            tree: vec![0; size + 1],
            size,
        }
    }

    /// Builds a Fenwick tree from an existing slice in O(n).
    pub fn from_slice(arr: &[i64]) -> Self {
        let size = arr.len();
        let mut ft = Self::new(size);
        for (i, &val) in arr.iter().enumerate() {
            ft.add(i, val);
        }
        ft
    }

    /// Adds delta to the element at 0-based index in O(log n).
    pub fn add(&mut self, index: usize, delta: i64) -> bool {
        if index >= self.size {
            return false;
        }
        let mut i = index + 1;
        while i <= self.size {
            self.tree[i] += delta;
            i += i & (!i + 1); // i & (-i)
        }
        true
    }

    /// Computes prefix sum of range [0, index] in O(log n).
    pub fn query(&self, index: usize) -> Option<i64> {
        if index >= self.size {
            return None;
        }
        let mut sum = 0;
        let mut i = index + 1;
        while i > 0 {
            sum += self.tree[i];
            i -= i & (!i + 1); // i & (-i)
        }
        Some(sum)
    }

    /// Computes range sum of range [left, right] inclusive in O(log n).
    pub fn range_query(&self, left: usize, right: usize) -> Option<i64> {
        if left > right || right >= self.size {
            return None;
        }
        let sum_r = self.query(right)?;
        let sum_l = if left > 0 { self.query(left - 1)? } else { 0 };
        Some(sum_r - sum_l)
    }

    pub fn len(&self) -> usize {
        self.size
    }

    pub fn is_empty(&self) -> bool {
        self.size == 0
    }
}
