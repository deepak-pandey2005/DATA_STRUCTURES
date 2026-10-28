/**
 * @file main.cpp
 * @brief Standalone runnable demonstration for Binary Search Tree in Modern C++20.
 */

#include <iostream>
#include "BinarySearchTree.hpp"

int main() {
    std::cout << "=== [06] Binary Search Tree Standalone Demo (Modern C++20) ===\n\n";

    ds::BinarySearchTree<int> bst;
    int sample_keys[] = {50, 30, 70, 20, 40, 60, 80};

    std::cout << "Inserting keys: ";
    for (int k : sample_keys) {
        std::cout << k << " ";
        bst.insert(k);
    }
    std::cout << "\nTree size: " << bst.size() << "\n";

    std::cout << "In-order traversal: [ ";
    bst.inorder([](const int &v) { std::cout << v << " "; });
    std::cout << "]\n";

    int search_key = 40;
    std::cout << "Contains " << search_key << ": " << (bst.contains(search_key) ? "YES" : "NO") << "\n";

    std::cout << "Deleting root node 50 (in-order successor replacement)...\n";
    bst.remove(50);

    std::cout << "In-order after deletion: [ ";
    bst.inorder([](const int &v) { std::cout << v << " "; });
    std::cout << "]\n";
    std::cout << "New size: " << bst.size() << "\n";

    return 0;
}
