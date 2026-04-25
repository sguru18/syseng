CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_structs_bin

all: $(TARGET)

$(TARGET): test_structs.c structs.c structs.h
	$(CC) $(CFLAGS) -o $@ test_structs.c structs.c

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
