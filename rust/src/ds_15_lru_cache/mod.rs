use std::collections::HashMap;
use std::hash::Hash;

struct Node<K, V> {
    key: K,
    value: V,
    prev: Option<usize>,
    next: Option<usize>,
}

/// An LRU (Least Recently Used) cache backed by a `HashMap` and an arena-allocated
/// doubly-linked list of nodes using array indices.
pub struct LruCache<K, V> {
    capacity: usize,
    map: HashMap<K, usize>,
    nodes: Vec<Option<Node<K, V>>>,
    free_indices: Vec<usize>,
    head: Option<usize>, // MRU node
    tail: Option<usize>, // LRU node
}

impl<K: Clone + Eq + Hash, V> LruCache<K, V> {
    /// Creates a new `LruCache` with the given capacity.
    ///
    /// # Panics
    /// Panics if capacity is 0.
    pub fn new(capacity: usize) -> Self {
        assert!(capacity > 0, "Capacity must be greater than zero");
        Self {
            capacity,
            map: HashMap::with_capacity(capacity),
            nodes: Vec::with_capacity(capacity),
            free_indices: Vec::new(),
            head: None,
            tail: None,
        }
    }

    /// Returns the capacity of the cache.
    pub fn capacity(&self) -> usize {
        self.capacity
    }

    /// Returns the number of elements in the cache.
    pub fn len(&self) -> usize {
        self.map.len()
    }

    /// Returns true if the cache contains no elements.
    pub fn is_empty(&self) -> bool {
        self.map.is_empty()
    }

    /// Returns a reference to the value associated with `key`, and marks it as MRU.
    pub fn get(&mut self, key: &K) -> Option<&V> {
        if let Some(&idx) = self.map.get(key) {
            self.move_to_head(idx);
            self.nodes[idx].as_ref().map(|node| &node.value)
        } else {
            None
        }
    }

    /// Returns a mutable reference to the value associated with `key`, and marks it as MRU.
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> {
        if let Some(&idx) = self.map.get(key) {
            self.move_to_head(idx);
            self.nodes[idx].as_mut().map(|node| &mut node.value)
        } else {
            None
        }
    }

    /// Inserts a key-value pair into the cache.
    /// If the cache is at capacity, the least recently used element is evicted.
    /// Returns the old value if the key was already present.
    pub fn put(&mut self, key: K, value: V) -> Option<V> {
        if let Some(&idx) = self.map.get(&key) {
            self.move_to_head(idx);
            let old_val = std::mem::replace(&mut self.nodes[idx].as_mut().unwrap().value, value);
            return Some(old_val);
        }

        if self.len() >= self.capacity {
            self.evict_lru();
        }

        let new_idx = self.allocate_node(key.clone(), value);
        self.add_to_head(new_idx);
        self.map.insert(key, new_idx);
        None
    }

    /// Checks whether the cache contains the given key without updating access recency.
    pub fn contains_key(&self, key: &K) -> bool {
        self.map.contains_key(key)
    }

    fn unlink(&mut self, idx: usize) {
        let (prev, next) = {
            let node = self.nodes[idx].as_ref().unwrap();
            (node.prev, node.next)
        };

        match prev {
            Some(p) => self.nodes[p].as_mut().unwrap().next = next,
            None => self.head = next,
        }

        match next {
            Some(n) => self.nodes[n].as_mut().unwrap().prev = prev,
            None => self.tail = prev,
        }

        let node = self.nodes[idx].as_mut().unwrap();
        node.prev = None;
        node.next = None;
    }

    fn add_to_head(&mut self, idx: usize) {
        let old_head = self.head;
        {
            let node = self.nodes[idx].as_mut().unwrap();
            node.next = old_head;
            node.prev = None;
        }

        match old_head {
            Some(h) => self.nodes[h].as_mut().unwrap().prev = Some(idx),
            None => self.tail = Some(idx),
        }

        self.head = Some(idx);
    }

    fn move_to_head(&mut self, idx: usize) {
        if self.head == Some(idx) {
            return;
        }
        self.unlink(idx);
        self.add_to_head(idx);
    }

    fn allocate_node(&mut self, key: K, value: V) -> usize {
        let node = Node {
            key,
            value,
            prev: None,
            next: None,
        };
        if let Some(idx) = self.free_indices.pop() {
            self.nodes[idx] = Some(node);
            idx
        } else {
            let idx = self.nodes.len();
            self.nodes.push(Some(node));
            idx
        }
    }

    fn evict_lru(&mut self) {
        if let Some(lru_idx) = self.tail {
            self.unlink(lru_idx);
            if let Some(node) = self.nodes[lru_idx].take() {
                self.map.remove(&node.key);
            }
            self.free_indices.push(lru_idx);
        }
    }
}
