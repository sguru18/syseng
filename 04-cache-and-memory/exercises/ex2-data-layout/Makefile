CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -march=native -I../../../tools
TARGET   := data_layout

.PHONY: all clean

all: $(TARGET)

$(TARGET): data_layout.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
