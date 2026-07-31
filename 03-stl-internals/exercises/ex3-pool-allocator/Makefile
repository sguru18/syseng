CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I../../../tools
TARGET   = test_pool_allocator_bin

all: $(TARGET)

$(TARGET): test_pool_allocator.cpp pool_allocator.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_pool_allocator.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
