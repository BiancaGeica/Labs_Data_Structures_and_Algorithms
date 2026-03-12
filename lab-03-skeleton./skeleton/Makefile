.phony: build test test-problem clean

TESTSRC = $(wildcard test*.c)
TEST = $(patsubst %.c,%,$(TESTSRC))

PROBSRC = problem.c
TESTPROB = $(patsubst %.c,%,$(PROBSRC))

SRC = $(foreach src,$(wildcard *c),$(patsubst test%,,$(src)))
EXEC = $(patsubst %.c,%,$(SRC))

HEADERS = $(wildcard *.h)

CC = gcc
CC_FLAGS = -std=c9x -g -O0

build: $(EXEC) $(TEST)

test: $(TEST)
	@valgrind --leak-check=full ./$(TEST) > test_stdout.tmp 2> test_valgrind.tmp; \
	cat test_stdout.tmp; \
	echo "========================================"; \
	echo "Verificare Valgrind:"; \
	if grep -q "All heap blocks were freed -- no leaks are possible" test_valgrind.tmp; then \
		echo "  PASSED - Fara memory leaks!"; \
	else \
		echo "  FAILED - Memory leaks detectate!"; \
		grep "definitely lost\|indirectly lost\|possibly lost" test_valgrind.tmp | sed 's/^/  /'; \
	fi; \
	echo "========================================"; \
	rm -f test_stdout.tmp test_valgrind.tmp

test-problem: $(TESTPROB)
	valgrind --leak-check=full ./$(TESTPROB)

$(EXEC):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@

$(TEST):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@

clean:
	rm -f $(EXEC) $(TEST) test_stdout.tmp test_valgrind.tmp
