CC      := clang
CFLAGS  := -Wall -Wextra -Werror -std=c17 -g -I../../../tools
TARGET  := test_sort
SRCS    := sort.c test_sort.c

$(TARGET): $(SRCS) sort.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: test clean
