CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools

TARGET   := test_server
SRCS     := test_server.cpp

.PHONY: all test clean

all: $(TARGET)

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS) server.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
