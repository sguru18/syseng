CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools

TARGET   := test_fix_parser
SRCS     := test_fix_parser.cpp

.PHONY: all test clean

all: $(TARGET)

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS) fix_parser.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
