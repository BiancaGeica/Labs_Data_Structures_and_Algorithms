.PHONY: build run test test-valgrind clean

CC = gcc
CFLAGS = -std=c11 -g -O0 -Wall -Wextra
LDFLAGS =

TARGET = testEx3

build: $(TARGET)

$(TARGET): testEx3.c bintree.c avl.c bintree.h avl.h OrderedSet.h
	$(CC) $(CFLAGS) testEx3.c bintree.c avl.c -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@./$(TARGET)

test-valgrind: $(TARGET)
	@valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
