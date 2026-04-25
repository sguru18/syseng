CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_strings_bin

all: $(TARGET)

$(TARGET): test_strings.c strings.c strings.h
	$(CC) $(CFLAGS) -o $@ test_strings.c strings.c

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
