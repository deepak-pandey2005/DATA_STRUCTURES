//! Binary Heap (Priority Queue) Implementation in safe Rust.
//!
//! Provides a standard max-heap by default using the `Ord` trait,
//! backed by a contiguous `Vec<T>`.

use std::cmp::Ord;

#[derive(Debug, Clone)]
pub struct BinaryHeap<T> {
    data: Vec<T>,
}

impl<T: Ord> BinaryHeap<T> {
    /// Creates an empty binary heap.
    pub fn new() -> Self {
        Self { data: Vec::new() }
    }

    /// Creates an empty binary heap with specific capacity.
    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Vec::with_capacity(capacity),
        }
    }

    /// Returns the number of elements in the heap.
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns true if the heap contains no elements.
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Returns a reference to the maximum element in O(1).
    pub fn peek(&self) -> Option<&T> {
        self.data.first()
    }

    /// Pushes an item onto the heap in O(log n).
    pub fn push(&mut self, item: T) {
        self.data.push(item);
        self.sift_up(self.data.len() - 1);
    }

    /// Removes and returns the maximum item in O(log n).
    pub fn pop(&mut self) -> Option<T> {
        if self.data.is_empty() {
            return None;
        }
        let last = self.data.pop()?;
        if self.data.is_empty() {
            Some(last)
        } else {
            let root = std::mem::replace(&mut self.data[0], last);
            self.sift_down(0);
            Some(root)
        }
    }

    fn sift_up(&mut self, mut idx: usize) {
        while idx > 0 {
            let parent = (idx - 1) / 2;
            if self.data[idx] > self.data[parent] {
                self.data.swap(idx, parent);
                idx = parent;
            } else {
                break;
            }
        }
    }

    fn sift_down(&mut self, mut idx: usize) {
        let len = self.data.len();
        while idx < len {
            let left = 2 * idx + 1;
            let right = 2 * idx + 2;
            let mut largest = idx;

            if left < len && self.data[left] > self.data[largest] {
                largest = left;
            }
            if right < len && self.data[right] > self.data[largest] {
                largest = right;
            }

            if largest != idx {
                self.data.swap(idx, largest);
                idx = largest;
            } else {
                break;
            }
        }
    }
}

impl<T: Ord> Default for BinaryHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Ord> From<Vec<T>> for BinaryHeap<T> {
    fn from(vec: Vec<T>) -> Self {
        if vec.len() > 1 {
            let last_parent = (vec.len() / 2) - 1;
            let mut heap = Self { data: vec };
            for i in (0..=last_parent).rev() {
                heap.sift_down(i);
            }
            return heap;
        }
        Self { data: vec }
    }
}
