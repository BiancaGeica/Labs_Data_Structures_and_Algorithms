.PHONY: build run test test-valgrind clean

CC = gcc
CFLAGS = -std=c11 -g -O0 -Wall -Wextra
LDFLAGS =

TARGET = testEx6

build: $(TARGET)

$(TARGET): testEx6.c merge_k.h SortedList.h Heap.h
	$(CC) $(CFLAGS) testEx6.c -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@./$(TARGET)

test-valgrind: $(TARGET)
	@valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
