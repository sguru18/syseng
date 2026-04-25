CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g -I../../../tools
TARGET = test_dns_bin

all: $(TARGET)

$(TARGET): test_dns.c dns.c dns.h
	$(CC) $(CFLAGS) -o $@ test_dns.c dns.c

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
