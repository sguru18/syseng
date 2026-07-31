CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I../../../tools
TARGET   := test_price_levels

.PHONY: all run clean

all: $(TARGET)

$(TARGET): test_price_levels.cpp price_levels.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
