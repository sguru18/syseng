CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_hello_bin

all: $(TARGET)

$(TARGET): test_hello.c hello.c hello.h
	$(CC) $(CFLAGS) -o $@ test_hello.c hello.c

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
