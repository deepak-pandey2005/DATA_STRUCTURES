//! Disjoint Set Union (Union-Find / DSU) implementation in safe Rust.
//!
//! Provides near O(1) amortized subset merging and root representative
//! lookups using path compression and union by rank.

#[derive(Debug, Clone)]
pub struct Dsu {
    parent: Vec<usize>,
    rank: Vec<usize>,
    num_sets: usize,
}

impl Dsu {
    /// Initializes DSU universe of size n with elements [0..n-1].
    pub fn new(n: usize) -> Self {
        Self {
            parent: (0..n).collect(),
            rank: vec![0; n],
            num_sets: n,
        }
    }

    /// Finds root representative with path compression.
    pub fn find(&mut self, mut x: usize) -> usize {
        let mut root = x;
        while root != self.parent[root] {
            root = self.parent[root];
        }

        // Path compression pass
        while x != root {
            let next = self.parent[x];
            self.parent[x] = root;
            x = next;
        }

        root
    }

    /// Merges sets containing x and y using union by rank.
    /// Returns true if two disjoint sets were merged, false if already in same set.
    pub fn union(&mut self, x: usize, y: usize) -> bool {
        let root_x = self.find(x);
        let root_y = self.find(y);

        if root_x == root_y {
            return false;
        }

        if self.rank[root_x] < self.rank[root_y] {
            self.parent[root_x] = root_y;
        } else if self.rank[root_x] > self.rank[root_y] {
            self.parent[root_y] = root_x;
        } else {
            self.parent[root_y] = root_x;
            self.rank[root_x] += 1;
        }

        self.num_sets -= 1;
        true
    }

    /// Checks if elements x and y are connected in the same set.
    pub fn connected(&mut self, x: usize, y: usize) -> bool {
        self.find(x) == self.find(y)
    }

    /// Current number of disjoint components/sets.
    pub fn set_count(&self) -> usize {
        self.num_sets
    }

    /// Universe size.
    pub fn len(&self) -> usize {
        self.parent.len()
    }

    /// Returns true if size is 0.
    pub fn is_empty(&self) -> bool {
        self.parent.is_empty()
    }
}
