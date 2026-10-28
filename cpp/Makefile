CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++20 \
           -I01_vector/include \
           -I02_singly_linked_list/include \
           -I03_doubly_linked_list/include \
           -I04_stack/include \
           -I05_queue/include \
           -I06_bst/include \
           -I07_heap/include \
           -I08_hash_table/include \
           -I09_trie/include \
           -I10_dsu/include \
           -I11_avl_tree/include \
           -I12_graph/include \
           -g

SRCS = 01_vector/src/Vector.cpp \
       02_singly_linked_list/src/SinglyLinkedList.cpp \
       03_doubly_linked_list/src/DoublyLinkedList.cpp \
       04_stack/src/Stack.cpp \
       05_queue/src/Queue.cpp \
       06_bst/src/BinarySearchTree.cpp \
       07_heap/src/BinaryHeap.cpp \
       08_hash_table/src/HashTable.cpp \
       09_trie/src/Trie.cpp \
       10_dsu/src/DSU.cpp \
       11_avl_tree/src/AVLTree.cpp \
       12_graph/src/Graph.cpp

TEST_SRCS = tests/test_main.cpp $(SRCS)
BIN = bin/test_runner

.PHONY: all test clean

all: $(BIN)

$(BIN): $(TEST_SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(BIN)
	@./$(BIN)

clean:
	rm -rf bin
