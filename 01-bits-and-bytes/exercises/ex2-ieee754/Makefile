CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_float_bits_bin

all: $(TARGET)

$(TARGET): test_float_bits.c float_bits.c float_bits.h
	$(CC) $(CFLAGS) -o $@ test_float_bits.c float_bits.c -lm

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
