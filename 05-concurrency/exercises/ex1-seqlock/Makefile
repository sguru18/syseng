CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools
LDFLAGS  := -lpthread

TARGET   := test_seqlock
SRCS     := test_seqlock.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) seqlock.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
