pub mod vector;
pub mod singly_linked_list;
pub mod doubly_linked_list;
pub mod stack;
pub mod queue;
pub mod bst;

pub use vector::MyVec;
pub use singly_linked_list::SinglyLinkedList;
pub use doubly_linked_list::DoublyLinkedList;
pub use stack::Stack;
pub use queue::Queue;
pub use bst::BinarySearchTree;

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
}
