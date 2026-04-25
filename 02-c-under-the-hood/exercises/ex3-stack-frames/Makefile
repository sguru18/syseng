CC      := clang
CFLAGS  := -Wall -Wextra -Werror -std=c17 -g -fno-omit-frame-pointer -I../../../tools
TARGET  := test_backtrace
SRCS    := backtrace.c test_backtrace.c

$(TARGET): $(SRCS) backtrace.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: test clean
