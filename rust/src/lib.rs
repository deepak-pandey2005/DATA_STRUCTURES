pub mod vector;
pub mod singly_linked_list;
pub mod doubly_linked_list;
pub mod stack;
pub mod queue;
pub mod bst;
pub mod heap;
pub mod hash_table;
pub mod trie;
pub mod dsu;
pub mod avl_tree;
pub mod graph;

pub use vector::MyVec;
pub use singly_linked_list::SinglyLinkedList;
pub use doubly_linked_list::DoublyLinkedList;
pub use stack::Stack;
pub use queue::Queue;
pub use bst::BinarySearchTree;
pub use heap::BinaryHeap;
pub use hash_table::HashTable;
pub use trie::Trie;
pub use dsu::Dsu;
pub use avl_tree::AvlTree;
pub use graph::Graph;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vector() {
        let mut v = MyVec::new();
        assert!(v.is_empty());
        for i in 0..20 {
            v.push(i);
        }
        assert_eq!(v.len(), 20);
        assert_eq!(v[5], 5);

        v.insert(5, 999);
        assert_eq!(v[5], 999);
        assert_eq!(v.len(), 21);

        let removed = v.remove(5);
        assert_eq!(removed, 999);
        assert_eq!(v.len(), 20);

        assert_eq!(v.pop(), Some(19));
        assert_eq!(v.len(), 19);
    }

    #[test]
    fn test_slist() {
        let mut list = SinglyLinkedList::new();
        list.push_back(10);
        list.push_back(20);
        list.push_front(5); // [5, 10, 20]

        assert_eq!(list.len(), 3);
        assert_eq!(list.peek_front(), Some(&5));
        assert!(list.contains(&10));
        assert!(!list.contains(&99));

        let items: Vec<i32> = list.iter().copied().collect();
        assert_eq!(items, vec![5, 10, 20]);

        list.reverse(); // [20, 10, 5]
        assert_eq!(list.pop_front(), Some(20));
        assert_eq!(list.pop_front(), Some(10));
        assert_eq!(list.pop_front(), Some(5));
        assert_eq!(list.pop_front(), None);
    }

    #[test]
    fn test_dlist() {
        let mut dlist = DoublyLinkedList::new();
        dlist.push_back(100);
        dlist.push_back(200);
        dlist.push_front(50); // [50, 100, 200]

        assert_eq!(dlist.len(), 3);
        assert_eq!(dlist.pop_front(), Some(50));
        assert_eq!(dlist.pop_back(), Some(200));
        assert_eq!(dlist.pop_front(), Some(100));
        assert!(dlist.is_empty());
    }

    #[test]
    fn test_stack() {
        let mut s = Stack::new();
        for i in 1..=5 {
            s.push(i);
        }
        assert_eq!(s.len(), 5);
        assert_eq!(s.peek(), Some(&5));
        assert_eq!(s.pop(), Some(5));
        assert_eq!(s.pop(), Some(4));
        assert_eq!(s.len(), 3);
    }

    #[test]
    fn test_queue() {
        let mut q = Queue::with_capacity(2);
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        q.enqueue(4);

        assert_eq!(q.len(), 4);
        assert_eq!(q.dequeue(), Some(1));
        assert_eq!(q.dequeue(), Some(2));

        q.enqueue(5);
        q.enqueue(6);

        let mut expected = 3;
        while let Some(val) = q.dequeue() {
            assert_eq!(val, expected);
            expected += 1;
        }
        assert_eq!(expected, 7);
    }

    #[test]
    fn test_bst() {
        let mut bst = BinarySearchTree::new();
        let keys = [50, 30, 70, 20, 40, 60, 80];
        for &k in &keys {
            assert!(bst.insert(k));
        }
        assert!(!bst.insert(50)); // duplicate
        assert_eq!(bst.len(), 7);

        assert!(bst.contains(&40));
        assert!(!bst.contains(&99));
        assert_eq!(bst.min(), Some(&20));
        assert_eq!(bst.max(), Some(&80));

        let inorder: Vec<i32> = bst.inorder().into_iter().copied().collect();
        assert_eq!(inorder, vec![20, 30, 40, 50, 60, 70, 80]);

        // Remove leaf
        assert!(bst.remove(&20));
        assert!(!bst.contains(&20));

        // Remove node with 2 children
        assert!(bst.remove(&50));
        assert!(!bst.contains(&50));
        assert_eq!(bst.len(), 5);

        let inorder_after: Vec<i32> = bst.inorder().into_iter().copied().collect();
        assert_eq!(inorder_after, vec![30, 40, 60, 70, 80]);
    }

    #[test]
    fn test_heap() {
        let mut heap = BinaryHeap::new();
        assert!(heap.is_empty());

        let nums = vec![15, 10, 20, 8, 12, 25, 30];
        for n in &nums {
            heap.push(*n);
        }
        assert_eq!(heap.len(), 7);
        assert_eq!(heap.peek(), Some(&30));

        let mut prev = 1000;
        while let Some(top) = heap.pop() {
            assert!(top <= prev);
            prev = top;
        }
        assert!(heap.is_empty());

        // Test heapify from vector
        let heap2 = BinaryHeap::from(nums);
        assert_eq!(heap2.len(), 7);
        assert_eq!(heap2.peek(), Some(&30));
    }

    #[test]
    fn test_hash_table() {
        let mut map = HashTable::with_capacity(4);
        assert!(map.is_empty());

        map.insert("apple".to_string(), 10);
        map.insert("banana".to_string(), 20);
        map.insert("cherry".to_string(), 30);
        map.insert("date".to_string(), 40);

        assert_eq!(map.len(), 4);
        assert_eq!(map.get("banana"), Some(&20));
        assert!(map.contains_key("apple"));
        assert!(!map.contains_key("grape"));

        // Update
        let old = map.insert("banana".to_string(), 200);
        assert_eq!(old, Some(20));
        assert_eq!(map.get("banana"), Some(&200));

        // Mutable access
        if let Some(val) = map.get_mut("date") {
            *val += 5;
        }
        assert_eq!(map.get("date"), Some(&45));

        // Remove
        assert_eq!(map.remove("cherry"), Some(30));
        assert!(!map.contains_key("cherry"));
        assert_eq!(map.len(), 3);
    }

    #[test]
    fn test_trie() {
        let mut trie = Trie::new();
        assert!(trie.is_empty());

        assert!(trie.insert("banana"));
        assert!(trie.insert("band"));
        assert!(trie.insert("banner"));
        assert!(!trie.insert("banana")); // duplicate

        assert_eq!(trie.len(), 3);
        assert!(trie.search("banana"));
        assert!(trie.search("band"));
        assert!(!trie.search("ban"));

        // Prefix
        assert!(trie.starts_with("ban"));
        assert!(trie.starts_with("band"));
        assert!(!trie.starts_with("zoo"));

        // Removal
        assert!(trie.remove("band"));
        assert!(!trie.search("band"));
        assert!(trie.search("banana"));
        assert_eq!(trie.len(), 2);
    }

    #[test]
    fn test_dsu() {
        let mut dsu = Dsu::new(6);
        assert_eq!(dsu.set_count(), 6);

        assert!(dsu.union(0, 1));
        assert!(dsu.union(1, 2));
        assert!(dsu.connected(0, 2));
        assert!(!dsu.connected(0, 3));
        assert_eq!(dsu.set_count(), 4);

        // Already connected
        assert!(!dsu.union(0, 2));
        assert_eq!(dsu.set_count(), 4);

        assert!(dsu.union(3, 4));
        assert!(dsu.union(4, 5));
        assert_eq!(dsu.set_count(), 2);

        // Merge components
        assert!(dsu.union(2, 4));
        assert_eq!(dsu.set_count(), 1);
        assert!(dsu.connected(0, 5));
    }

    #[test]
    fn test_avl_tree() {
        let mut avl = AvlTree::new();
        assert!(avl.is_empty());

        let vals = [10, 20, 30, 40, 50, 25, 5];
        for v in vals {
            assert!(avl.insert(v));
        }
        assert!(!avl.insert(20)); // duplicate
        assert_eq!(avl.len(), 7);
        assert!(avl.height() <= 4); // strictly logarithmic balance!

        assert!(avl.contains(&25));
        assert!(avl.contains(&10));
        assert!(!avl.contains(&999));

        let inorder = avl.inorder();
        assert_eq!(inorder, vec![5, 10, 20, 25, 30, 40, 50]);

        // Remove node with rotation rebalance
        assert!(avl.remove(&30));
        assert!(!avl.contains(&30));
        assert_eq!(avl.len(), 6);
        assert!(avl.height() <= 4);
    }

    #[test]
    fn test_graph() {
        let mut g = Graph::new(5, false); // undirected
        assert_eq!(g.vertex_count(), 5);
        assert!(!g.is_directed());

        g.add_edge(0, 1, 1);
        g.add_edge(1, 2, 1);
        g.add_edge(2, 3, 1);
        g.add_edge(3, 0, 1);
        g.add_edge(2, 4, 1);

        assert!(g.has_edge(0, 1));
        assert!(g.has_edge(1, 0));
        assert!(g.has_edge(2, 4));
        assert!(!g.has_edge(0, 4));

        let bfs_res = g.bfs(0);
        assert_eq!(bfs_res.len(), 5);
        assert_eq!(bfs_res[0], 0);

        let dfs_res = g.dfs(0);
        assert_eq!(dfs_res.len(), 5);
        assert_eq!(dfs_res[0], 0);
    }
}
