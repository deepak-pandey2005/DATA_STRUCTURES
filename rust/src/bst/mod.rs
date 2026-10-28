pub struct BinarySearchTree<T: Ord> {
    root: Option<Box<Node<T>>>,
    len: usize,
}

struct Node<T> {
    val: T,
    left: Option<Box<Node<T>>>,
    right: Option<Box<Node<T>>>,
}

impl<T: Ord> BinarySearchTree<T> {
    pub fn new() -> Self {
        Self { root: None, len: 0 }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn insert(&mut self, val: T) -> bool {
        let mut curr = &mut self.root;
        while let Some(ref mut node) = curr {
            if val < node.val {
                curr = &mut node.left;
            } else if val > node.val {
                curr = &mut node.right;
            } else {
                return false; // duplicate
            }
        }

        *curr = Some(Box::new(Node {
            val,
            left: None,
            right: None,
        }));
        self.len += 1;
        true
    }

    pub fn contains(&self, val: &T) -> bool {
        let mut curr = self.root.as_deref();
        while let Some(node) = curr {
            if val < &node.val {
                curr = node.left.as_deref();
            } else if val > &node.val {
                curr = node.right.as_deref();
            } else {
                return true;
            }
        }
        false
    }

    pub fn min(&self) -> Option<&T> {
        let mut curr = self.root.as_deref();
        while let Some(node) = curr {
            if node.left.is_none() {
                return Some(&node.val);
            }
            curr = node.left.as_deref();
        }
        None
    }

    pub fn max(&self) -> Option<&T> {
        let mut curr = self.root.as_deref();
        while let Some(node) = curr {
            if node.right.is_none() {
                return Some(&node.val);
            }
            curr = node.right.as_deref();
        }
        None
    }

    pub fn height(&self) -> usize {
        fn node_height<T>(node: Option<&Node<T>>) -> usize {
            match node {
                Some(n) => 1 + std::cmp::max(node_height(n.left.as_deref()), node_height(n.right.as_deref())),
                None => 0,
            }
        }
        node_height(self.root.as_deref())
    }

    pub fn inorder(&self) -> Vec<&T> {
        let mut result = Vec::with_capacity(self.len);
        fn traverse<'a, T>(node: Option<&'a Node<T>>, acc: &mut Vec<&'a T>) {
            if let Some(n) = node {
                traverse(n.left.as_deref(), acc);
                acc.push(&n.val);
                traverse(n.right.as_deref(), acc);
            }
        }
        traverse(self.root.as_deref(), &mut result);
        result
    }

    pub fn remove(&mut self, val: &T) -> bool {
        fn remove_rec<T: Ord>(root: Option<Box<Node<T>>>, val: &T, removed: &mut bool) -> Option<Box<Node<T>>> {
            let mut node = root?;
            if val < &node.val {
                node.left = remove_rec(node.left, val, removed);
                Some(node)
            } else if val > &node.val {
                node.right = remove_rec(node.right, val, removed);
                Some(node)
            } else {
                *removed = true;
                match (node.left.take(), node.right.take()) {
                    (None, None) => None,
                    (Some(left), None) => Some(left),
                    (None, Some(right)) => Some(right),
                    (Some(left), Some(right)) => {
                        let (new_right, min_val) = remove_min(right);
                        let new_node = Box::new(Node {
                            val: min_val,
                            left: Some(left),
                            right: new_right,
                        });
                        Some(new_node)
                    }
                }
            }
        }

        fn remove_min<T: Ord>(mut node: Box<Node<T>>) -> (Option<Box<Node<T>>>, T) {
            if let Some(left) = node.left.take() {
                let (new_left, min_val) = remove_min(left);
                node.left = new_left;
                (Some(node), min_val)
            } else {
                (node.right.take(), node.val)
            }
        }

        let mut removed = false;
        self.root = remove_rec(self.root.take(), val, &mut removed);
        if removed {
            self.len -= 1;
        }
        removed
    }
}

impl<T: Ord> Default for BinarySearchTree<T> {
    fn default() -> Self {
        Self::new()
    }
}
