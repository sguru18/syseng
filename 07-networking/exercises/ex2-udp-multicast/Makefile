CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools

TARGET   := test_multicast
SRCS     := test_multicast.cpp

.PHONY: all test clean

all: $(TARGET)

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS) multicast.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
