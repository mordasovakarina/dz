CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = test_hashtable

all: $(TARGET)

$(TARGET): HashTable.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) HashTable.o -L. -lHashTest

HashTable.o: HashTable.cpp HashTable.h
	$(CXX) $(CXXFLAGS) -c HashTable.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean run
