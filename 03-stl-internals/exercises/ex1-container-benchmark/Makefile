CXX      = clang++
BOOST_INC = /opt/homebrew/include
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I../../../tools -I$(BOOST_INC)
TARGET   = bench_containers_bin

all: $(TARGET)

$(TARGET): bench_containers.cpp
	$(CXX) $(CXXFLAGS) -o $@ bench_containers.cpp

test: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -rf *.dSYM

.PHONY: all test clean
