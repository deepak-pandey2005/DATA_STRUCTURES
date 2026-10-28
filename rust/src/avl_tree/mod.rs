//! Self-Balancing AVL Tree implementation in safe Rust.
//!
//! Maintains O(log n) worst-case time complexity for search, insertion,
//! and deletion using cached subtree heights and rotation rebalancing.

use std::cmp::Ordering;

#[derive(Debug, Clone)]
struct AvlNode<T> {
    data: T,
    height: usize,
    left: Option<Box<AvlNode<T>>>,
    right: Option<Box<AvlNode<T>>>,
}

impl<T: Ord> AvlNode<T> {
    fn new(data: T) -> Self {
        Self {
            data,
            height: 1,
            left: None,
            right: None,
        }
    }

    fn height(node: &Option<Box<AvlNode<T>>>) -> usize {
        node.as_ref().map_or(0, |n| n.height)
    }

    fn update_height(&mut self) {
        self.height = 1 + std::cmp::max(Self::height(&self.left), Self::height(&self.right));
    }

    fn balance_factor(&self) -> isize {
        Self::height(&self.left) as isize - Self::height(&self.right) as isize
    }

    fn rotate_right(mut root: Box<AvlNode<T>>) -> Box<AvlNode<T>> {
        let mut new_root = root.left.take().expect("rotate_right requires left child");
        root.left = new_root.right.take();
        root.update_height();
        new_root.right = Some(root);
        new_root.update_height();
        new_root
    }

    fn rotate_left(mut root: Box<AvlNode<T>>) -> Box<AvlNode<T>> {
        let mut new_root = root.right.take().expect("rotate_left requires right child");
        root.right = new_root.left.take();
        root.update_height();
        new_root.left = Some(root);
        new_root.update_height();
        new_root
    }

    fn balance(mut root: Box<AvlNode<T>>) -> Box<AvlNode<T>> {
        root.update_height();
        let bf = root.balance_factor();

        // Left heavy
        if bf > 1 {
            if root.left.as_ref().unwrap().balance_factor() < 0 {
                let left = root.left.take().unwrap();
                root.left = Some(Self::rotate_left(left));
            }
            return Self::rotate_right(root);
        }

        // Right heavy
        if bf < -1 {
            if root.right.as_ref().unwrap().balance_factor() > 0 {
                let right = root.right.take().unwrap();
                root.right = Some(Self::rotate_right(right));
            }
            return Self::rotate_left(root);
        }

        root
    }

    fn insert(node: Option<Box<AvlNode<T>>>, data: T, inserted: &mut bool) -> Option<Box<AvlNode<T>>> {
        let mut curr = match node {
            None => {
                *inserted = true;
                return Some(Box::new(AvlNode::new(data)));
            }
            Some(n) => n,
        };

        match data.cmp(&curr.data) {
            Ordering::Less => {
                curr.left = Self::insert(curr.left.take(), data, inserted);
            }
            Ordering::Greater => {
                curr.right = Self::insert(curr.right.take(), data, inserted);
            }
            Ordering::Equal => {
                *inserted = false;
                return Some(curr);
            }
        }

        Some(Self::balance(curr))
    }

    fn min_value(node: &AvlNode<T>) -> &T {
        match &node.left {
            Some(left) => Self::min_value(left),
            None => &node.data,
        }
    }

    fn remove(node: Option<Box<AvlNode<T>>>, data: &T, removed: &mut bool) -> Option<Box<AvlNode<T>>>
    where
        T: Clone,
    {
        let mut curr = match node {
            None => return None,
            Some(n) => n,
        };

        match data.cmp(&curr.data) {
            Ordering::Less => {
                curr.left = Self::remove(curr.left.take(), data, removed);
            }
            Ordering::Greater => {
                curr.right = Self::remove(curr.right.take(), data, removed);
            }
            Ordering::Equal => {
                *removed = true;
                if curr.left.is_none() {
                    return curr.right;
                }
                if curr.right.is_none() {
                    return curr.left;
                }

                let succ_val = Self::min_value(curr.right.as_ref().unwrap()).clone();
                curr.data = succ_val.clone();
                curr.right = Self::remove(curr.right.take(), &succ_val, &mut false);
            }
        }

        Some(Self::balance(curr))
    }
}

#[derive(Debug, Clone, Default)]
pub struct AvlTree<T> {
    root: Option<Box<AvlNode<T>>>,
    size: usize,
}

impl<T: Ord + Clone> AvlTree<T> {
    /// Creates an empty AVL tree.
    pub fn new() -> Self {
        Self { root: None, size: 0 }
    }

    /// Inserts a value into the tree. Returns true if inserted.
    pub fn insert(&mut self, data: T) -> bool {
        let mut inserted = false;
        self.root = AvlNode::insert(self.root.take(), data, &mut inserted);
        if inserted {
            self.size += 1;
        }
        inserted
    }

    /// Removes a value from the tree. Returns true if removed.
    pub fn remove(&mut self, data: &T) -> bool {
        let mut removed = false;
        self.root = AvlNode::remove(self.root.take(), data, &mut removed);
        if removed {
            self.size -= 1;
        }
        removed
    }

    /// Searches for a value.
    pub fn contains(&self, data: &T) -> bool {
        let mut curr = &self.root;
        while let Some(node) = curr {
            match data.cmp(&node.data) {
                Ordering::Less => curr = &node.left,
                Ordering::Greater => curr = &node.right,
                Ordering::Equal => return true,
            }
        }
        false
    }

    /// Returns height of tree.
    pub fn height(&self) -> usize {
        AvlNode::height(&self.root)
    }

    /// Total number of nodes.
    pub fn len(&self) -> usize {
        self.size
    }

    /// Returns true if empty.
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    /// In-order traversal yielding a vector of elements.
    pub fn inorder(&self) -> Vec<T> {
        let mut res = Vec::with_capacity(self.size);
        fn traverse<T: Clone>(node: &Option<Box<AvlNode<T>>>, res: &mut Vec<T>) {
            if let Some(n) = node {
                traverse(&n.left, res);
                res.push(n.data.clone());
                traverse(&n.right, res);
            }
        }
        traverse(&self.root, &mut res);
        res
    }
}

