CXX      := clang++
CXXFLAGS := -std=c++17 -O2 -I../../../tools
LDFLAGS  := -lpthread

TARGET   := test_spsc_queue
SRCS     := test_spsc_queue.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) spsc_queue.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
