CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -g -I../../../tools

test: test_pool
	@echo "──── Pool Allocator Tests ────"
	@./test_pool

test_pool: test_pool.c pool.c pool.h
	$(CC) $(CFLAGS) -o $@ test_pool.c pool.c

clean:
	rm -f test_pool

.PHONY: test clean
