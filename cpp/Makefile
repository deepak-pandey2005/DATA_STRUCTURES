CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++20 -Iinclude -g

SRCS = src/Vector.cpp \
       src/SinglyLinkedList.cpp \
       src/DoublyLinkedList.cpp \
       src/Stack.cpp \
       src/Queue.cpp \
       src/BinarySearchTree.cpp

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
