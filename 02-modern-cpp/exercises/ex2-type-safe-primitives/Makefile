CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I../../../tools
TARGET   = test_primitives_bin

all: $(TARGET)

$(TARGET): test_primitives.cpp primitives.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_primitives.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
