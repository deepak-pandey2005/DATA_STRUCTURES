CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinclude -g

SRCS = src/vector.c \
       src/singly_linked_list.c \
       src/doubly_linked_list.c \
       src/stack.c \
       src/queue.c \
       src/bst.c

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
