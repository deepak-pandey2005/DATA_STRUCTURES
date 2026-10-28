//! Adjacency List Graph with BFS and DFS in safe Rust.

use std::collections::VecDeque;

#[derive(Debug, Clone)]
pub struct Edge<W> {
    pub to: usize,
    pub weight: W,
}

#[derive(Debug, Clone)]
pub struct Graph<W = i32> {
    adj: Vec<Vec<Edge<W>>>,
    directed: bool,
}

impl<W: Clone> Graph<W> {
    /// Creates a graph with n vertices.
    pub fn new(num_vertices: usize, directed: bool) -> Self {
        Self {
            adj: vec![Vec::new(); num_vertices],
            directed,
        }
    }

    /// Adds an edge from src to dest with weight.
    pub fn add_edge(&mut self, src: usize, dest: usize, weight: W) {
        assert!(src < self.adj.len() && dest < self.adj.len(), "Vertex out of bounds");
        self.adj[src].push(Edge { to: dest, weight: weight.clone() });
        if !self.directed && src != dest {
            self.adj[dest].push(Edge { to: src, weight });
        }
    }

    /// Checks if an edge exists from src to dest.
    pub fn has_edge(&self, src: usize, dest: usize) -> bool {
        if src >= self.adj.len() || dest >= self.adj.len() {
            return false;
        }
        self.adj[src].iter().any(|e| e.to == dest)
    }

    /// Performs Breadth-First Search (BFS) from start, returning traversal order.
    pub fn bfs(&self, start: usize) -> Vec<usize> {
        let mut visited = vec![false; self.adj.len()];
        let mut order = Vec::new();
        let mut queue = VecDeque::new();

        visited[start] = true;
        queue.push_back(start);

        while let Some(u) = queue.pop_front() {
            order.push(u);
            for edge in &self.adj[u] {
                if !visited[edge.to] {
                    visited[edge.to] = true;
                    queue.push_back(edge.to);
                }
            }
        }

        order
    }

    /// Performs Depth-First Search (DFS) from start, returning traversal order.
    pub fn dfs(&self, start: usize) -> Vec<usize> {
        let mut visited = vec![false; self.adj.len()];
        let mut order = Vec::new();
        self.dfs_helper(start, &mut visited, &mut order);
        order
    }

    fn dfs_helper(&self, u: usize, visited: &mut [bool], order: &mut Vec<usize>) {
        visited[u] = true;
        order.push(u);

        for edge in &self.adj[u] {
            if !visited[edge.to] {
                self.dfs_helper(edge.to, visited, order);
            }
        }
    }

    pub fn vertex_count(&self) -> usize {
        self.adj.len()
    }

    pub fn is_directed(&self) -> bool {
        self.directed
    }
}
