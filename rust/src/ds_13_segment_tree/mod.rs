//! Segment Tree in safe Rust.
//!
//! Provides O(log n) associative range queries and point updates.

pub struct SegmentTree<T, F> {
    n: usize,
    tree: Vec<T>,
    identity: T,
    op: F,
}

impl<T: Clone, F: Fn(&T, &T) -> T> SegmentTree<T, F> {
    /// Builds a Segment Tree from a slice, an identity element, and an associative operation.
    pub fn new(arr: &[T], identity: T, op: F) -> Self {
        let n = arr.len();
        if n == 0 {
            return Self {
                n: 0,
                tree: Vec::new(),
                identity,
                op,
            };
        }
        let mut st = Self {
            n,
            tree: vec![identity.clone(); 4 * n],
            identity,
            op,
        };
        st.build(arr, 0, 0, n - 1);
        st
    }

    fn build(&mut self, arr: &[T], node: usize, start: usize, end: usize) {
        if start == end {
            self.tree[node] = arr[start].clone();
            return;
        }
        let mid = start + (end - start) / 2;
        let left_child = 2 * node + 1;
        let right_child = 2 * node + 2;

        self.build(arr, left_child, start, mid);
        self.build(arr, right_child, mid + 1, end);
        self.tree[node] = (self.op)(&self.tree[left_child], &self.tree[right_child]);
    }

    /// Queries associative range query [left, right] inclusive in O(log n).
    pub fn query(&self, left: usize, right: usize) -> Option<T> {
        if left > right || right >= self.n {
            return None;
        }
        Some(self.query_helper(0, 0, self.n - 1, left, right))
    }

    fn query_helper(&self, node: usize, start: usize, end: usize, l: usize, r: usize) -> T {
        if r < start || end < l {
            return self.identity.clone();
        }
        if l <= start && end <= r {
            return self.tree[node].clone();
        }
        let mid = start + (end - start) / 2;
        let left_res = self.query_helper(2 * node + 1, start, mid, l, r);
        let right_res = self.query_helper(2 * node + 2, mid + 1, end, l, r);
        (self.op)(&left_res, &right_res)
    }

    /// Updates value at index in O(log n).
    pub fn update(&mut self, index: usize, new_val: T) -> bool {
        if index >= self.n {
            return false;
        }
        self.update_helper(0, 0, self.n - 1, index, new_val);
        true
    }

    fn update_helper(&mut self, node: usize, start: usize, end: usize, idx: usize, val: T) {
        if start == end {
            self.tree[node] = val;
            return;
        }
        let mid = start + (end - start) / 2;
        let left_child = 2 * node + 1;
        let right_child = 2 * node + 2;

        if idx <= mid {
            self.update_helper(left_child, start, mid, idx, val);
        } else {
            self.update_helper(right_child, mid + 1, end, idx, val);
        }
        self.tree[node] = (self.op)(&self.tree[left_child], &self.tree[right_child]);
    }

    pub fn len(&self) -> usize {
        self.n
    }

    pub fn is_empty(&self) -> bool {
        self.n == 0
    }
}
