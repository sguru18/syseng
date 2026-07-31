CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I../../../tools
TARGET   = test_circular_buffer_bin

all: $(TARGET)

$(TARGET): test_circular_buffer.cpp circular_buffer.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_circular_buffer.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
