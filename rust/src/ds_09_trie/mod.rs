//! Trie (Prefix Tree) implementation in safe Rust.
//!
//! Provides prefix-based string searching, insertion, and word removal
//! using nested `Option<Box<TrieNode>>` 26-element alphabet arrays.

const ALPHABET_SIZE: usize = 26;

#[derive(Debug, Clone)]
struct TrieNode {
    children: [Option<Box<TrieNode>>; ALPHABET_SIZE],
    is_end: bool,
}

impl TrieNode {
    fn new() -> Self {
        const INIT: Option<Box<TrieNode>> = None;
        Self {
            children: [INIT; ALPHABET_SIZE],
            is_end: false,
        }
    }

    fn has_children(&self) -> bool {
        self.children.iter().any(|c| c.is_some())
    }
}

impl Default for TrieNode {
    fn default() -> Self {
        Self::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct Trie {
    root: TrieNode,
    word_count: usize,
}

impl Trie {
    /// Creates an empty Trie.
    pub fn new() -> Self {
        Self {
            root: TrieNode::new(),
            word_count: 0,
        }
    }

    fn char_to_index(ch: char) -> Option<usize> {
        let lower = ch.to_ascii_lowercase();
        if lower.is_ascii_lowercase() {
            Some((lower as u8 - b'a') as usize)
        } else {
            None
        }
    }

    /// Inserts a word consisting of ASCII characters into the Trie.
    pub fn insert(&mut self, word: &str) -> bool {
        let mut curr = &mut self.root;
        for ch in word.chars() {
            let idx = match Self::char_to_index(ch) {
                Some(i) => i,
                None => return false,
            };
            if curr.children[idx].is_none() {
                curr.children[idx] = Some(Box::new(TrieNode::new()));
            }
            curr = curr.children[idx].as_mut().unwrap();
        }

        if !curr.is_end {
            curr.is_end = true;
            self.word_count += 1;
            true
        } else {
            false
        }
    }

    /// Checks if exact word exists in the Trie.
    pub fn search(&self, word: &str) -> bool {
        let mut curr = &self.root;
        for ch in word.chars() {
            let idx = match Self::char_to_index(ch) {
                Some(i) => i,
                None => return false,
            };
            match &curr.children[idx] {
                Some(next_node) => curr = next_node,
                None => return false,
            }
        }
        curr.is_end
    }

    /// Checks if any word starts with prefix.
    pub fn starts_with(&self, prefix: &str) -> bool {
        let mut curr = &self.root;
        for ch in prefix.chars() {
            let idx = match Self::char_to_index(ch) {
                Some(i) => i,
                None => return false,
            };
            match &curr.children[idx] {
                Some(next_node) => curr = next_node,
                None => return false,
            }
        }
        true
    }

    /// Removes a word from the Trie.
    pub fn remove(&mut self, word: &str) -> bool {
        let mut removed = false;
        Self::remove_helper(&mut self.root, word.as_bytes(), 0, &mut removed);
        if removed {
            self.word_count -= 1;
        }
        removed
    }

    fn remove_helper(node: &mut TrieNode, bytes: &[u8], depth: usize, removed: &mut bool) -> bool {
        if depth == bytes.len() {
            if node.is_end {
                node.is_end = false;
                *removed = true;
            }
            return !node.has_children();
        }

        let ch = bytes[depth] as char;
        let idx = match Self::char_to_index(ch) {
            Some(i) => i,
            None => return false,
        };

        if let Some(ref mut child) = node.children[idx] {
            if Self::remove_helper(child, bytes, depth + 1, removed) {
                node.children[idx] = None;
                return !node.is_end && !node.has_children();
            }
        }
        false
    }

    /// Total number of words in the Trie.
    pub fn len(&self) -> usize {
        self.word_count
    }

    /// Returns true if empty.
    pub fn is_empty(&self) -> bool {
        self.word_count == 0
    }
}
