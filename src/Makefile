# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -c     - compile only (don't link). This is used to compile a c/cpp file into an object (.o) file.
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -std=c++17

# The build target
TARGET = main
OBJECTS = main.o features.o preproc.o trie.o keywords.o
RM = del

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o: features.h main.cpp
	$(CC) $(CFLAGS) -c main.cpp

features.o: preproc.h features.h features.cpp
	$(CC) $(CFLAGS) -c features.cpp

preproc.o: keywords.h trie.h preproc.h preproc.cpp
	$(CC) $(CFLAGS) -c preproc.cpp

trie.o: trie.h trie.cpp
	$(CC) $(CFLAGS) -c trie.cpp

keywords.o: keywords.h keywords.cpp
	$(CC) $(CFLAGS) -c keywords.cpp

clean:
	$(RM) -f $(OBJECTS)
	$(RM) -f $(TARGET)