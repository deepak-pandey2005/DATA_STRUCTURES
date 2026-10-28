# Trie (Prefix Tree)

A **Trie** (derived from re**trie**val) is an efficient tree-like search structure specialized for dynamic sets or associative arrays where keys are usually strings.

---

## 1. Core Mechanics & Architecture

Unlike binary search trees, no node in the Trie stores the entire key. Instead, a node's position in the tree defines the key with which it is associated.

### Structure
- Each edge represents a character (typically lowercase alphabet `'a'` through `'z'`).
- Each node has:
  - An array or map of children pointers (size = 26 for standard ASCII English alphabet).
  - A boolean flag `is_end_of_word` indicating whether the path from the root to this node constitutes a valid word.

```
          (root)
         /      \
       'a'      't'
       /          \
     'p'          'o' (is_end=true) -> "to"
     /            /  \
   'p'          'e'  'o' (is_end=true) -> "too"
   /              \
 'l'              'a' (is_end=true) -> "tea"
 /
'e' (is_end=true) -> "apple"
```

---

## 2. Operations & Complexity Analysis

Let $L$ be the length of the string / word:

| Operation | Time Complexity | Space Complexity | Description |
| :--- | :--- | :--- | :--- |
| **Insert** | $O(L)$ | $O(L \times \Sigma)$ | Traverse existing nodes or allocate missing characters along path; mark terminal node `is_end=true`. |
| **Search** | $O(L)$ | $O(1)$ | Follow child pointers matching each character; verify terminal node has `is_end=true`. |
| **Starts With** | $O(L)$ | $O(1)$ | Verify if there exists a valid path matching the prefix string. |
| **Remove** | $O(L)$ | $O(1)$ | Unset `is_end`; recursively prune childless, non-terminal branch nodes. |

Where $\Sigma$ is the alphabet size ($\Sigma = 26$).

---

## 3. Language Implementation Details

- **C (`c/trie/`)**: Fixed 26-pointer array per node for $O(1)$ direct child indexing (`c - 'a'`) with recursive post-order destructor.
- **C++ (`cpp/trie/`)**: `Trie` class utilizing `std::unique_ptr` arrays for memory safety and zero leaks under exceptions.
- **Rust (`rust/src/trie/`)**: Idiomatic Rust implementation with nested `Option<Box<TrieNode>>` fixed arrays.

