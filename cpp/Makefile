CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++20 \
           -Ivector/include \
           -Isingly_linked_list/include \
           -Idoubly_linked_list/include \
           -Istack/include \
           -Iqueue/include \
           -Ibst/include \
           -Iheap/include \
           -Ihash_table/include \
           -Itrie/include \
           -Idsu/include \
           -g

SRCS = vector/src/Vector.cpp \
       singly_linked_list/src/SinglyLinkedList.cpp \
       doubly_linked_list/src/DoublyLinkedList.cpp \
       stack/src/Stack.cpp \
       queue/src/Queue.cpp \
       bst/src/BinarySearchTree.cpp \
       heap/src/BinaryHeap.cpp \
       hash_table/src/HashTable.cpp \
       trie/src/Trie.cpp \
       dsu/src/DSU.cpp

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
