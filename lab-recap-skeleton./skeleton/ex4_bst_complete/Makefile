.PHONY: build run test test-valgrind clean

CC = gcc
CFLAGS = -std=c11 -g -O0 -Wall -Wextra
LDFLAGS =

TARGET = testEx4

build: $(TARGET)

$(TARGET): testEx4.c tree.c bst_utils.c tree.h
	$(CC) $(CFLAGS) testEx4.c tree.c bst_utils.c -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@./$(TARGET)

test-valgrind: $(TARGET)
	@valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
