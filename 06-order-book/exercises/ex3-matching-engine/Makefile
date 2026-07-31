CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra \
            -I../../../tools \
            -I../ex1-price-levels \
            -I../ex2-limit-order-book
TARGET   := test_matching_engine

.PHONY: all run clean

all: $(TARGET)

$(TARGET): test_matching_engine.cpp matching_engine.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
