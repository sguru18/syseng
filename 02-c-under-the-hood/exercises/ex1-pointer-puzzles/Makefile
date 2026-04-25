CC      := clang
CFLAGS  := -Wall -Wextra -Werror -std=c17 -g -I../../../tools
TARGET  := test_puzzles
SRCS    := puzzles.c test_puzzles.c

$(TARGET): $(SRCS) puzzles.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: test clean
