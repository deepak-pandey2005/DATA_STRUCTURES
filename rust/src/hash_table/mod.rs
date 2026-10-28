//! Hash Table (Hash Map) with Separate Chaining in safe Rust.
//!
//! Provides amortized O(1) key-value lookup, insertion, and removal
//! utilizing Rust's `std::hash::Hash` and `std::collections::hash_map::DefaultHasher`.

use std::collections::hash_map::DefaultHasher;
use std::hash::{Hash, Hasher};

const DEFAULT_CAPACITY: usize = 16;
const MAX_LOAD_FACTOR: f64 = 0.75;

#[derive(Debug, Clone)]
pub struct HashTable<K, V> {
    buckets: Vec<Vec<(K, V)>>,
    size: usize,
}

impl<K: Hash + Eq + Clone, V: Clone> HashTable<K, V> {
    /// Creates an empty hash table with default capacity.
    pub fn new() -> Self {
        Self::with_capacity(DEFAULT_CAPACITY)
    }

    /// Creates an empty hash table with specified capacity.
    pub fn with_capacity(capacity: usize) -> Self {
        let cap = if capacity > 0 { capacity } else { DEFAULT_CAPACITY };
        let mut buckets = Vec::with_capacity(cap);
        for _ in 0..cap {
            buckets.push(Vec::new());
        }
        Self { buckets, size: 0 }
    }

    fn hash_key<Q: ?Sized + Hash>(&self, key: &Q) -> usize {
        let mut hasher = DefaultHasher::new();
        key.hash(&mut hasher);
        (hasher.finish() as usize) % self.buckets.len()
    }

    /// Inserts or updates a key-value pair.
    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        if (self.size + 1) as f64 / self.buckets.len() as f64 >= MAX_LOAD_FACTOR {
            self.rehash(self.buckets.len() * 2);
        }

        let idx = self.hash_key(&key);
        for entry in &mut self.buckets[idx] {
            if entry.0 == key {
                return Some(std::mem::replace(&mut entry.1, value));
            }
        }

        self.buckets[idx].push((key, value));
        self.size += 1;
        None
    }

    /// Retrieves an immutable reference to the value associated with key.
    pub fn get<Q: ?Sized + Hash + Eq>(&self, key: &Q) -> Option<&V>
    where
        K: std::borrow::Borrow<Q>,
    {
        if self.size == 0 {
            return None;
        }
        let idx = self.hash_key(key);
        for entry in &self.buckets[idx] {
            if entry.0.borrow() == key {
                return Some(&entry.1);
            }
        }
        None
    }

    /// Retrieves a mutable reference to the value associated with key.
    pub fn get_mut<Q: ?Sized + Hash + Eq>(&mut self, key: &Q) -> Option<&mut V>
    where
        K: std::borrow::Borrow<Q>,
    {
        if self.size == 0 {
            return None;
        }
        let idx = self.hash_key(key);
        for entry in &mut self.buckets[idx] {
            if entry.0.borrow() == key {
                return Some(&mut entry.1);
            }
        }
        None
    }

    /// Checks whether the table contains key.
    pub fn contains_key<Q: ?Sized + Hash + Eq>(&self, key: &Q) -> bool
    where
        K: std::borrow::Borrow<Q>,
    {
        self.get(key).is_some()
    }

    /// Removes an entry matching key and returns its value.
    pub fn remove<Q: ?Sized + Hash + Eq>(&mut self, key: &Q) -> Option<V>
    where
        K: std::borrow::Borrow<Q>,
    {
        if self.size == 0 {
            return None;
        }
        let idx = self.hash_key(key);
        let bucket = &mut self.buckets[idx];
        if let Some(pos) = bucket.iter().position(|e| e.0.borrow() == key) {
            let (_, val) = bucket.swap_remove(pos);
            self.size -= 1;
            Some(val)
        } else {
            None
        }
    }

    /// Number of entries stored.
    pub fn len(&self) -> usize {
        self.size
    }

    /// Returns true if empty.
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    fn rehash(&mut self, new_cap: usize) {
        let mut new_buckets = Vec::with_capacity(new_cap);
        for _ in 0..new_cap {
            new_buckets.push(Vec::new());
        }

        for bucket in &mut self.buckets {
            for (k, v) in bucket.drain(..) {
                let mut hasher = DefaultHasher::new();
                k.hash(&mut hasher);
                let idx = (hasher.finish() as usize) % new_cap;
                new_buckets[idx].push((k, v));
            }
        }

        self.buckets = new_buckets;
    }
}

impl<K: Hash + Eq + Clone, V: Clone> Default for HashTable<K, V> {
    fn default() -> Self {
        Self::new()
    }
}

