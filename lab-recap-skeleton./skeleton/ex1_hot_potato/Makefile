.PHONY: build run test test-valgrind clean

CC = gcc
CFLAGS = -std=c11 -g -O0 -Wall -Wextra
LDFLAGS =

TARGET = testEx1

build: $(TARGET)

$(TARGET): testEx1.c hot_potato.c hot_potato.h Queue.h
	$(CC) $(CFLAGS) testEx1.c hot_potato.c -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@./$(TARGET)

test-valgrind: $(TARGET)
	@valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
