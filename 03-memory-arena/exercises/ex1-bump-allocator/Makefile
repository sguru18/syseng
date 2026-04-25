CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -g -I../../../tools

test: test_bump
	@echo "──── Bump Allocator Tests ────"
	@./test_bump

test_bump: test_bump.c bump.c bump.h
	$(CC) $(CFLAGS) -o $@ test_bump.c bump.c

clean:
	rm -f test_bump

.PHONY: test clean
