CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = splay

all: $(TARGET)

$(TARGET): splay_tree_api.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) splay_tree_api.o

splay_tree_api.o: splay_tree_api.cpp splay_tree_api.h
	$(CXX) $(CXXFLAGS) -c splay_tree_api.cpp -o splay_tree_api.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f splay_tree_api.o $(TARGET)

.PHONY: all clean run
