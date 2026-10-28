/**
 * @file playground.cpp
 * @brief Interactive CLI Playground for experimenting with all 12 Data Structures in Modern C++20.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Vector.hpp"
#include "SinglyLinkedList.hpp"
#include "DoublyLinkedList.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearchTree.hpp"
#include "BinaryHeap.hpp"
#include "HashTable.hpp"
#include "Trie.hpp"
#include "DSU.hpp"
#include "AVLTree.hpp"
#include "Graph.hpp"

static void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void run_vector_demo() {
    std::cout << "\n--- [01] Vector Playground ---\n";
    ds::Vector<int> vec;
    int choice = 0;
    while (true) {
        std::cout << "\nVector size: " << vec.size() << ", capacity: " << vec.capacity() << "\n";
        std::cout << "Elements: [ ";
        for (std::size_t i = 0; i < vec.size(); ++i) std::cout << vec[i] << " ";
        std::cout << "]\n";
        std::cout << "1. Push Back | 2. Pop Back | 3. Insert At | 4. Remove At | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            int val;
            std::cout << "Enter integer to push: ";
            if (std::cin >> val) vec.push_back(val);
        } else if (choice == 2) {
            if (!vec.empty()) {
                std::cout << "Popped: " << vec.back() << "\n";
                vec.pop_back();
            } else std::cout << "Vector is empty!\n";
        } else if (choice == 3) {
            std::size_t idx;
            int val;
            std::cout << "Enter index and value: ";
            if (std::cin >> idx >> val && idx <= vec.size()) vec.insert(idx, val);
            else std::cout << "Invalid index!\n";
        } else if (choice == 4) {
            std::size_t idx;
            std::cout << "Enter index to remove: ";
            if (std::cin >> idx && idx < vec.size()) vec.erase(idx);
            else std::cout << "Invalid index!\n";
        }
    }
}

static void run_bst_demo() {
    std::cout << "\n--- [06] Binary Search Tree Playground ---\n";
    ds::BinarySearchTree<int> bst;
    int choice = 0;
    while (true) {
        std::cout << "\nBST size: " << bst.size() << "\nIn-order traversal: [ ";
        bst.inorder([](const int &v) { std::cout << v << " "; });
        std::cout << "]\n";
        std::cout << "1. Insert | 2. Search | 3. Remove | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            int val;
            std::cout << "Enter value to insert: ";
            if (std::cin >> val) {
                if (bst.insert(val)) std::cout << "Inserted " << val << "\n";
                else std::cout << val << " already exists!\n";
            }
        } else if (choice == 2) {
            int val;
            std::cout << "Enter value to search: ";
            if (std::cin >> val) {
                std::cout << (bst.contains(val) ? "Found!" : "Not found!") << "\n";
            }
        } else if (choice == 3) {
            int val;
            std::cout << "Enter value to remove: ";
            if (std::cin >> val) {
                std::cout << (bst.remove(val) ? "Removed!" : "Key not found!") << "\n";
            }
        }
    }
}

static void run_avl_demo() {
    std::cout << "\n--- [11] AVL Tree (Self-Balancing) Playground ---\n";
    ds::AVLTree<int> avl;
    int choice = 0;
    while (true) {
        std::cout << "\nAVL size: " << avl.size() << ", Height: " << avl.height() << "\nIn-order: [ ";
        for (int v : avl.inorder()) std::cout << v << " ";
        std::cout << "]\n";
        std::cout << "1. Insert (auto-rotate) | 2. Remove (auto-rotate) | 3. Contains | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            int val;
            std::cout << "Enter value: ";
            if (std::cin >> val) {
                if (avl.insert(val)) std::cout << "Inserted " << val << " (tree rebalanced)\n";
                else std::cout << "Duplicate value!\n";
            }
        } else if (choice == 2) {
            int val;
            std::cout << "Enter value: ";
            if (std::cin >> val) {
                std::cout << (avl.remove(val) ? "Removed and rebalanced!" : "Not found!") << "\n";
            }
        } else if (choice == 3) {
            int val;
            std::cout << "Enter value: ";
            if (std::cin >> val) {
                std::cout << (avl.contains(val) ? "Found!" : "Not found!") << "\n";
            }
        }
    }
}

static void run_heap_demo() {
    std::cout << "\n--- [07] Binary Max-Heap Playground ---\n";
    ds::BinaryHeap<int> heap;
    int choice = 0;
    while (true) {
        std::cout << "\nHeap size: " << heap.size();
        if (!heap.empty()) std::cout << ", Top: " << heap.top();
        std::cout << "\n1. Push | 2. Pop Top | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            int val;
            std::cout << "Enter value: ";
            if (std::cin >> val) heap.push(val);
        } else if (choice == 2) {
            if (!heap.empty()) {
                std::cout << "Extracted max: " << heap.top() << "\n";
                heap.pop();
            } else std::cout << "Heap is empty!\n";
        }
    }
}

static void run_hash_table_demo() {
    std::cout << "\n--- [08] Hash Table Playground ---\n";
    ds::HashTable<std::string, std::string> map;
    int choice = 0;
    while (true) {
        std::cout << "\nHash Table entries: " << map.size() << "\n";
        std::cout << "1. Put (Key-Value) | 2. Get | 3. Erase | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            std::string k, v;
            std::cout << "Enter key and value (space-separated): ";
            if (std::cin >> k >> v) {
                map.insert_or_assign(k, v);
                std::cout << "Stored [" << k << " -> " << v << "]\n";
            }
        } else if (choice == 2) {
            std::string k;
            std::cout << "Enter key: ";
            if (std::cin >> k) {
                auto *val = map.find(k);
                if (val) std::cout << "Value: " << *val << "\n";
                else std::cout << "Key not found!\n";
            }
        } else if (choice == 3) {
            std::string k;
            std::cout << "Enter key: ";
            if (std::cin >> k) {
                std::cout << (map.erase(k) ? "Deleted!\n" : "Key not found!\n");
            }
        }
    }
}

static void run_trie_demo() {
    std::cout << "\n--- [09] Trie (Prefix Tree) Playground ---\n";
    ds::Trie trie;
    int choice = 0;
    while (true) {
        std::cout << "\nTrie distinct words: " << trie.size() << "\n";
        std::cout << "1. Insert Word | 2. Exact Search | 3. Starts With | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            std::string w;
            std::cout << "Enter word: ";
            if (std::cin >> w) {
                if (trie.insert(w)) std::cout << "Inserted \"" << w << "\"\n";
                else std::cout << "Word already exists!\n";
            }
        } else if (choice == 2) {
            std::string w;
            std::cout << "Enter word to search: ";
            if (std::cin >> w) {
                std::cout << (trie.search(w) ? "Exact word found!" : "Not in trie!") << "\n";
            }
        } else if (choice == 3) {
            std::string p;
            std::cout << "Enter prefix: ";
            if (std::cin >> p) {
                std::cout << (trie.starts_with(p) ? "Prefix exists!" : "No words start with this prefix!") << "\n";
            }
        }
    }
}

static void run_dsu_demo() {
    std::cout << "\n--- [10] Disjoint Set Union (DSU) Playground ---\n";
    std::size_t n = 6;
    ds::DSU dsu(n);
    int choice = 0;
    while (true) {
        std::cout << "\nDSU Universe [0.." << n - 1 << "], Disjoint components: " << dsu.set_count() << "\n";
        std::cout << "1. Unite (x, y) | 2. Check Connected (x, y) | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            std::size_t x, y;
            std::cout << "Enter elements x and y: ";
            if (std::cin >> x >> y && x < n && y < n) {
                if (dsu.unite(x, y)) std::cout << "Merged sets of " << x << " and " << y << "\n";
                else std::cout << "Already in the same set!\n";
            } else std::cout << "Invalid elements!\n";
        } else if (choice == 2) {
            std::size_t x, y;
            std::cout << "Enter elements x and y: ";
            if (std::cin >> x >> y && x < n && y < n) {
                std::cout << (dsu.connected(x, y) ? "Connected (Same set)!" : "Disconnected (Different sets)!") << "\n";
            }
        }
    }
}

static void run_graph_demo() {
    std::cout << "\n--- [12] Graph (Adjacency List) Playground ---\n";
    ds::Graph<int> g(6, false); // 6 vertices undirected
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(1, 4);
    std::cout << "Initialized sample graph with 6 vertices and 6 edges.\n";

    int choice = 0;
    while (true) {
        std::cout << "\n1. Add Edge | 2. BFS Traversal | 3. DFS Traversal | 0. Back: ";
        if (!(std::cin >> choice) || choice == 0) break;
        if (choice == 1) {
            std::size_t u, v;
            std::cout << "Enter vertices u and v (0..5): ";
            if (std::cin >> u >> v && u < 6 && v < 6) {
                g.add_edge(u, v);
                std::cout << "Edge added between " << u << " and " << v << "\n";
            }
        } else if (choice == 2) {
            std::size_t start;
            std::cout << "Enter start vertex (0..5): ";
            if (std::cin >> start && start < 6) {
                std::cout << "BFS order: [ ";
                g.bfs(start, [](std::size_t node) { std::cout << node << " "; });
                std::cout << "]\n";
            }
        } else if (choice == 3) {
            std::size_t start;
            std::cout << "Enter start vertex (0..5): ";
            if (std::cin >> start && start < 6) {
                std::cout << "DFS order: [ ";
                g.dfs(start, [](std::size_t node) { std::cout << node << " "; });
                std::cout << "]\n";
            }
        }
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  Interactive Data Structures Playground (Modern C++20) \n";
    std::cout << "========================================================\n";

    int menu = 0;
    while (true) {
        std::cout << "\nSelect a Data Structure to experiment with:\n";
        std::cout << "  1. [01] Vector (Dynamic Array)\n";
        std::cout << "  2. [06] Binary Search Tree (BST)\n";
        std::cout << "  3. [07] Binary Max-Heap (Priority Queue)\n";
        std::cout << "  4. [08] Hash Table (Separate Chaining)\n";
        std::cout << "  5. [09] Trie (Prefix Tree)\n";
        std::cout << "  6. [10] Disjoint Set Union (DSU)\n";
        std::cout << "  7. [11] AVL Tree (Self-Balancing)\n";
        std::cout << "  8. [12] Graph (Adjacency List + BFS/DFS)\n";
        std::cout << "  0. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> menu) || menu == 0) {
            std::cout << "\nExiting playground. Goodbye!\n";
            break;
        }

        switch (menu) {
            case 1: run_vector_demo(); break;
            case 2: run_bst_demo(); break;
            case 3: run_heap_demo(); break;
            case 4: run_hash_table_demo(); break;
            case 5: run_trie_demo(); break;
            case 6: run_dsu_demo(); break;
            case 7: run_avl_demo(); break;
            case 8: run_graph_demo(); break;
            default:
                std::cout << "Invalid choice!\n";
                clear_input();
                break;
        }
    }
    return 0;
}
