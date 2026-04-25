.PHONY: build run test test-valgrind clean

CC = gcc
CFLAGS = -std=c11 -g -O0 -Wall -Wextra
LDFLAGS =

TARGET = testEx5

build: $(TARGET)

$(TARGET): testEx5.c swap_pairs.h DoubleLinkedList.h
	$(CC) $(CFLAGS) testEx5.c -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@./$(TARGET)

test-valgrind: $(TARGET)
	@valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
