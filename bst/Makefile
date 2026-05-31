CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = run_tests

all: $(TARGET)

$(TARGET): BST.cpp BST.h
	$(CXX) $(CXXFLAGS) BST.cpp -o $(TARGET) -L. -lBSTTest

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean run