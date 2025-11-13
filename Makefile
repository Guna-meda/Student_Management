CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
SRC     = main.c book.c member.c reservation.c search.c report.c \
          datastructures/avl.c datastructures/hashmap.c \
          datastructures/trie.c datastructures/graph.c \
          datastructures/priorityqueue.c utils.c
OBJ     = $(SRC:.c=.o)
BIN     = libsys

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: clean