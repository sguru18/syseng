CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra \
            -I../../../tools \
            -I../ex1-price-levels
TARGET   := test_order_book

.PHONY: all run clean

all: $(TARGET)

$(TARGET): test_order_book.cpp order_book.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
