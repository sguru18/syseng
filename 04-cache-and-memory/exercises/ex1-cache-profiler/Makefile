CXX      := clang++
CXXFLAGS := -std=c++17 -O1 -fno-inline -I../../../tools
TARGET   := cache_profiler

.PHONY: all clean

all: $(TARGET)

$(TARGET): cache_profiler.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
