CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I../../../tools
TARGET   = test_ownership_bin

all: $(TARGET)

$(TARGET): test_ownership.cpp ownership.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_ownership.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
