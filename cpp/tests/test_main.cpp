#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include "Vector.hpp"
#include "SinglyLinkedList.hpp"
#include "DoublyLinkedList.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearchTree.hpp"

static int g_cpp_tests_passed = 0;

#define CPP_TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::exit(1); \
    } \
} while(0)

#define RUN_CPP_TEST(fn) do { \
    std::cout << "Running " << #fn << "... "; \
    fn(); \
    g_cpp_tests_passed++; \
    std::cout << "PASSED\n"; \
} while(0)

void test_vector_cpp() {
    ds::Vector<std::string> vec;
    CPP_TEST_ASSERT(vec.empty());

    vec.push_back("hello");
    vec.push_back("world");
    vec.push_back("modern");
    vec.push_back("c++");

    CPP_TEST_ASSERT(vec.size() == 4);
    CPP_TEST_ASSERT(vec[0] == "hello");
    CPP_TEST_ASSERT(vec.at(3) == "c++");
    CPP_TEST_ASSERT(vec.data() != nullptr);

    // Copy constructor
    ds::Vector<std::string> vec_copy = vec;
    CPP_TEST_ASSERT(vec_copy.size() == 4);
    CPP_TEST_ASSERT(vec_copy[1] == "world");

    // Move constructor
    ds::Vector<std::string> vec_moved = std::move(vec_copy);
    CPP_TEST_ASSERT(vec_moved.size() == 4);
    CPP_TEST_ASSERT(vec_copy.empty());

    // Range-based for loop & iterators
    int count = 0;
    for (const auto &str : vec_moved) {
        CPP_TEST_ASSERT(!str.empty());
        count++;
    }
    CPP_TEST_ASSERT(count == 4);
    CPP_TEST_ASSERT(vec_moved.cbegin() != vec_moved.cend());

    // Insert & Erase
    vec_moved.insert(1, "inserted");
    CPP_TEST_ASSERT(vec_moved[1] == "inserted");
    CPP_TEST_ASSERT(vec_moved.size() == 5);

    vec_moved.erase(1);
    CPP_TEST_ASSERT(vec_moved[1] == "world");
    CPP_TEST_ASSERT(vec_moved.size() == 4);

    // Exception handling
    bool caught = false;
    try {
        vec_moved.at(100);
    } catch (const std::out_of_range &) {
        caught = true;
    }
    CPP_TEST_ASSERT(caught);
}

void test_slist_cpp() {
    ds::SinglyLinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_front(5); // [5, 10, 20]

    CPP_TEST_ASSERT(list.size() == 3);
    CPP_TEST_ASSERT(list.front() == 5);
    CPP_TEST_ASSERT(list.back() == 20);
    CPP_TEST_ASSERT(list.contains(10));
    CPP_TEST_ASSERT(!list.contains(99));

    // Index access
    CPP_TEST_ASSERT(list.get(0) == 5);
    CPP_TEST_ASSERT(list[1] == 10);
    CPP_TEST_ASSERT(list[2] == 20);

    // insert_at & remove_at
    list.insert_at(1, 999); // [5, 999, 10, 20]
    CPP_TEST_ASSERT(list[1] == 999);
    CPP_TEST_ASSERT(list.size() == 4);

    CPP_TEST_ASSERT(list.remove_at(1) == 999); // back to [5, 10, 20]
    CPP_TEST_ASSERT(list.size() == 3);

    // remove_value
    CPP_TEST_ASSERT(list.remove_value(10)); // [5, 20]
    CPP_TEST_ASSERT(!list.contains(10));
    CPP_TEST_ASSERT(list.size() == 2);

    // Reverse
    list.reverse(); // [20, 5]
    CPP_TEST_ASSERT(list.front() == 20);
    CPP_TEST_ASSERT(list.back() == 5);

    CPP_TEST_ASSERT(list.pop_front() == 20);
    CPP_TEST_ASSERT(list.pop_back() == 5);
    CPP_TEST_ASSERT(list.empty());
}

void test_dlist_cpp() {
    ds::DoublyLinkedList<int> dlist;
    dlist.push_back(100);
    dlist.push_back(200);
    dlist.push_front(50); // [50, 100, 200]

    CPP_TEST_ASSERT(dlist.size() == 3);
    CPP_TEST_ASSERT(dlist.front() == 50);
    CPP_TEST_ASSERT(dlist.back() == 200);
    CPP_TEST_ASSERT(dlist[1] == 100);

    // Bidirectional iterator verification
    auto it = dlist.begin();
    CPP_TEST_ASSERT(*it == 50);
    ++it;
    CPP_TEST_ASSERT(*it == 100);
    ++it;
    CPP_TEST_ASSERT(*it == 200);
    --it;
    CPP_TEST_ASSERT(*it == 100);

    // insert_at & remove_at
    dlist.insert_at(1, 75); // [50, 75, 100, 200]
    CPP_TEST_ASSERT(dlist[1] == 75);
    CPP_TEST_ASSERT(dlist.size() == 4);

    CPP_TEST_ASSERT(dlist.remove_at(1) == 75);
    CPP_TEST_ASSERT(dlist.size() == 3);

    CPP_TEST_ASSERT(dlist.remove_first(100));
    CPP_TEST_ASSERT(dlist.size() == 2);

    CPP_TEST_ASSERT(dlist.pop_front() == 50);
    CPP_TEST_ASSERT(dlist.pop_back() == 200);
    CPP_TEST_ASSERT(dlist.empty());
}

void test_stack_cpp() {
    ds::Stack<int> s;
    CPP_TEST_ASSERT(s.empty());

    for (int i = 1; i <= 5; ++i) {
        s.push(i);
    }
    CPP_TEST_ASSERT(s.size() == 5);
    CPP_TEST_ASSERT(s.top() == 5);

    CPP_TEST_ASSERT(s.pop() == 5);
    CPP_TEST_ASSERT(s.pop() == 4);
    CPP_TEST_ASSERT(s.size() == 3);

    s.clear();
    CPP_TEST_ASSERT(s.empty());

    bool underflow = false;
    try {
        s.pop();
    } catch (const std::underflow_error &) {
        underflow = true;
    }
    CPP_TEST_ASSERT(underflow);
}

void test_queue_cpp() {
    ds::Queue<int> q(2);
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);

    CPP_TEST_ASSERT(q.size() == 4);
    CPP_TEST_ASSERT(q.front() == 1);
    CPP_TEST_ASSERT(q.back() == 4);

    CPP_TEST_ASSERT(q.dequeue() == 1);
    CPP_TEST_ASSERT(q.dequeue() == 2);

    q.enqueue(5);
    q.enqueue(6);

    int expected = 3;
    while (!q.empty()) {
        CPP_TEST_ASSERT(q.dequeue() == expected);
        expected++;
    }
    CPP_TEST_ASSERT(expected == 7);
}

void test_bst_cpp() {
    ds::BinarySearchTree<int> bst;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys) {
        CPP_TEST_ASSERT(bst.insert(k));
    }
    CPP_TEST_ASSERT(!bst.insert(50)); // duplicate
    CPP_TEST_ASSERT(bst.size() == 7);

    CPP_TEST_ASSERT(bst.contains(40));
    CPP_TEST_ASSERT(!bst.contains(999));
    CPP_TEST_ASSERT(bst.min() == 20);
    CPP_TEST_ASSERT(bst.max() == 80);
    CPP_TEST_ASSERT(bst.height() == 3);

    // In-order traversal
    std::vector<int> expected_inorder = {20, 30, 40, 50, 60, 70, 80};
    CPP_TEST_ASSERT(bst.inorder_keys() == expected_inorder);

    // Pre-order traversal
    std::vector<int> expected_preorder = {50, 30, 20, 40, 70, 60, 80};
    CPP_TEST_ASSERT(bst.preorder_keys() == expected_preorder);

    // Post-order traversal
    std::vector<int> expected_postorder = {20, 40, 30, 60, 80, 70, 50};
    CPP_TEST_ASSERT(bst.postorder_keys() == expected_postorder);

    // Level-order BFS traversal
    std::vector<int> expected_level_order = {50, 30, 70, 20, 40, 60, 80};
    CPP_TEST_ASSERT(bst.level_order_keys() == expected_level_order);

    // Remove leaf
    CPP_TEST_ASSERT(bst.remove(20));
    CPP_TEST_ASSERT(!bst.contains(20));

    // Remove node with 2 children
    CPP_TEST_ASSERT(bst.remove(50));
    CPP_TEST_ASSERT(!bst.contains(50));
    CPP_TEST_ASSERT(bst.size() == 5);

    // Deep copy test
    ds::BinarySearchTree<int> bst_copy = bst;
    CPP_TEST_ASSERT(bst_copy.size() == 5);
    CPP_TEST_ASSERT(bst_copy.contains(70));
}

int main() {
    std::cout << "==========================================\n";
    std::cout << "   Running C++ Data Structures Test Suite \n";
    std::cout << "==========================================\n";

    RUN_CPP_TEST(test_vector_cpp);
    RUN_CPP_TEST(test_slist_cpp);
    RUN_CPP_TEST(test_dlist_cpp);
    RUN_CPP_TEST(test_stack_cpp);
    RUN_CPP_TEST(test_queue_cpp);
    RUN_CPP_TEST(test_bst_cpp);

    std::cout << "\nAll " << g_cpp_tests_passed << " C++ tests passed successfully!\n";
    return 0;
}
