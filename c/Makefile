CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 \
         -Icommon/include \
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

SRCS = 01_vector/src/vector.c \
       02_singly_linked_list/src/singly_linked_list.c \
       03_doubly_linked_list/src/doubly_linked_list.c \
       04_stack/src/stack.c \
       05_queue/src/queue.c \
       06_bst/src/bst.c \
       07_heap/src/heap.c \
       08_hash_table/src/hash_table.c \
       09_trie/src/trie.c \
       10_dsu/src/dsu.c \
       11_avl_tree/src/avl_tree.c \
       12_graph/src/graph.c

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
