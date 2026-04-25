CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_bits_bin

all: $(TARGET)

$(TARGET): test_bits.c bits.c bits.h
	$(CC) $(CFLAGS) -o $@ test_bits.c bits.c

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
