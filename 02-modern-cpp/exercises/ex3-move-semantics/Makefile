CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2 -I../../../tools
TARGET   = test_buffer_bin

all: $(TARGET)

$(TARGET): test_buffer.cpp buffer.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_buffer.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
