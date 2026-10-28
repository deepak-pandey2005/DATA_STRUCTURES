CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 \
         -Icommon/include \
         -Ivector/include \
         -Isingly_linked_list/include \
         -Idoubly_linked_list/include \
         -Istack/include \
         -Iqueue/include \
         -Ibst/include \
         -Iheap/include \
         -Ihash_table/include \
         -Itrie/include \
         -g

SRCS = vector/src/vector.c \
       singly_linked_list/src/singly_linked_list.c \
       doubly_linked_list/src/doubly_linked_list.c \
       stack/src/stack.c \
       queue/src/queue.c \
       bst/src/bst.c \
       heap/src/heap.c \
       hash_table/src/hash_table.c \
       trie/src/trie.c

TEST_SRCS = tests/test_runner.c $(SRCS)
TEST_BIN = bin/test_runner

.PHONY: all test clean

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRCS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

clean:
	rm -rf bin
