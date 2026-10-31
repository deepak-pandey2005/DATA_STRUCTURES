const MAX_LEVEL: usize = 16;

struct Node<K, V> {
    key: K,
    value: V,
    forward: Vec<Option<usize>>,
}

/// A probabilistic ordered map backed by a multi-level Skip List.
pub struct SkipList<K, V> {
    head_forward: Vec<Option<usize>>,
    nodes: Vec<Option<Node<K, V>>>,
    free_indices: Vec<usize>,
    level: usize,
    size: usize,
    rng_state: u64,
}

impl<K: Ord + Clone, V> SkipList<K, V> {
    /// Creates an empty `SkipList`.
    pub fn new() -> Self {
        Self {
            head_forward: vec![None; MAX_LEVEL],
            nodes: Vec::new(),
            free_indices: Vec::new(),
            level: 1,
            size: 0,
            rng_state: 88172645463325252,
        }
    }

    /// Returns the number of elements in the skip list.
    pub fn len(&self) -> usize {
        self.size
    }

    /// Returns true if the skip list contains no elements.
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    fn xorshift64(&mut self) -> u64 {
        let mut x = self.rng_state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        self.rng_state = x;
        x
    }

    fn random_level(&mut self) -> usize {
        let mut lvl = 1;
        while (self.xorshift64() & 1) == 1 && lvl < MAX_LEVEL {
            lvl += 1;
        }
        lvl
    }

    /// Searches for a value associated with the given key.
    pub fn get(&self, key: &K) -> Option<&V> {
        let mut curr_opt: Option<usize> = None;

        for i in (0..self.level).rev() {
            loop {
                let next_idx = match curr_opt {
                    None => self.head_forward[i],
                    Some(c) => self.nodes[c].as_ref().unwrap().forward[i],
                };

                match next_idx {
                    Some(n) => {
                        let node_key = &self.nodes[n].as_ref().unwrap().key;
                        if node_key < key {
                            curr_opt = Some(n);
                        } else {
                            break;
                        }
                    }
                    None => break,
                }
            }
        }

        let target_idx = match curr_opt {
            None => self.head_forward[0],
            Some(c) => self.nodes[c].as_ref().unwrap().forward[0],
        };

        if let Some(idx) = target_idx {
            let node = self.nodes[idx].as_ref().unwrap();
            if node.key == *key {
                return Some(&node.value);
            }
        }

        None
    }

    /// Inserts a key-value pair into the skip list.
    /// Returns the old value if the key already existed.
    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        let mut update: [Option<usize>; MAX_LEVEL] = [None; MAX_LEVEL];
        let mut curr_opt: Option<usize> = None;

        for i in (0..self.level).rev() {
            loop {
                let next_idx = match curr_opt {
                    None => self.head_forward[i],
                    Some(c) => self.nodes[c].as_ref().unwrap().forward[i],
                };

                match next_idx {
                    Some(n) => {
                        let node_key = &self.nodes[n].as_ref().unwrap().key;
                        if node_key < &key {
                            curr_opt = Some(n);
                        } else {
                            break;
                        }
                    }
                    None => break,
                }
            }
            update[i] = curr_opt;
        }

        let target_idx = match curr_opt {
            None => self.head_forward[0],
            Some(c) => self.nodes[c].as_ref().unwrap().forward[0],
        };

        if let Some(idx) = target_idx {
            if self.nodes[idx].as_ref().unwrap().key == key {
                let old_val = std::mem::replace(&mut self.nodes[idx].as_mut().unwrap().value, value);
                return Some(old_val);
            }
        }

        let new_level = self.random_level();
        if new_level > self.level {
            for i in self.level..new_level {
                update[i] = None;
            }
            self.level = new_level;
        }

        let new_node = Node {
            key,
            value,
            forward: vec![None; new_level],
        };

        let new_idx = if let Some(idx) = self.free_indices.pop() {
            self.nodes[idx] = Some(new_node);
            idx
        } else {
            let idx = self.nodes.len();
            self.nodes.push(Some(new_node));
            idx
        };

        for i in 0..new_level {
            let next = match update[i] {
                None => self.head_forward[i],
                Some(u) => self.nodes[u].as_ref().unwrap().forward[i],
            };
            self.nodes[new_idx].as_mut().unwrap().forward[i] = next;

            match update[i] {
                None => self.head_forward[i] = Some(new_idx),
                Some(u) => self.nodes[u].as_mut().unwrap().forward[i] = Some(new_idx),
            }
        }

        self.size += 1;
        None
    }

    /// Removes a key from the skip list.
    /// Returns the removed value if found.
    pub fn remove(&mut self, key: &K) -> Option<V> {
        let mut update: [Option<usize>; MAX_LEVEL] = [None; MAX_LEVEL];
        let mut curr_opt: Option<usize> = None;

        for i in (0..self.level).rev() {
            loop {
                let next_idx = match curr_opt {
                    None => self.head_forward[i],
                    Some(c) => self.nodes[c].as_ref().unwrap().forward[i],
                };

                match next_idx {
                    Some(n) => {
                        let node_key = &self.nodes[n].as_ref().unwrap().key;
                        if node_key < key {
                            curr_opt = Some(n);
                        } else {
                            break;
                        }
                    }
                    None => break,
                }
            }
            update[i] = curr_opt;
        }

        let target_idx = match curr_opt {
            None => self.head_forward[0],
            Some(c) => self.nodes[c].as_ref().unwrap().forward[0],
        };

        let target = target_idx?;
        if self.nodes[target].as_ref().unwrap().key != *key {
            return None;
        }

        for i in 0..self.level {
            let matches = match update[i] {
                None => self.head_forward[i] == Some(target),
                Some(u) => self.nodes[u].as_ref().unwrap().forward[i] == Some(target),
            };

            if !matches {
                break;
            }

            let next = self.nodes[target].as_ref().unwrap().forward[i];
            match update[i] {
                None => self.head_forward[i] = next,
                Some(u) => self.nodes[u].as_mut().unwrap().forward[i] = next,
            }
        }

        while self.level > 1 && self.head_forward[self.level - 1].is_none() {
            self.level -= 1;
        }

        let removed_node = self.nodes[target].take().unwrap();
        self.free_indices.push(target);
        self.size -= 1;

        Some(removed_node.value)
    }
}

impl<K: Ord + Clone, V> Default for SkipList<K, V> {
    fn default() -> Self {
        Self::new()
    }
}
