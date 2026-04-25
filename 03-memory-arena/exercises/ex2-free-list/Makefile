CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -g -I../../../tools

test: test_freelist
	@echo "──── Free List Allocator Tests ────"
	@./test_freelist

test_freelist: test_freelist.c freelist.c freelist.h
	$(CC) $(CFLAGS) -o $@ test_freelist.c freelist.c

clean:
	rm -f test_freelist

.PHONY: test clean
