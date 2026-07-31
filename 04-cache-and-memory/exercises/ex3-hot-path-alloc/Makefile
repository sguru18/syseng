CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools
TARGET   := test_hot_alloc

.PHONY: all clean

all: $(TARGET)

$(TARGET): test_hot_alloc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
