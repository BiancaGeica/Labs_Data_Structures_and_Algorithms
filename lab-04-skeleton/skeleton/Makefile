.phony: build test-stack test-queue test-parantheses test-radix test clean

TESTSRC = $(wildcard test*.c)
TEST = $(patsubst %.c,%,$(TESTSRC))

SRC = $(foreach src,$(wildcard *c),$(patsubst test%,,$(src)))
EXEC = $(patsubst %.c,%,$(SRC))

HEADERS = $(wildcard *.h)

CC = gcc
CC_FLAGS = -std=c9x -g -O0
LDFLAGS = -lm

UNAME_S := $(shell uname -s)

build: $(EXEC) $(TEST)

ifeq ($(UNAME_S),Darwin)
define run_test
	@./$(1) > test_stdout.tmp 2> test_stderr.tmp; \
	exit_code=$$?; \
	if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		echo ""; \
		cat test_stderr.tmp 2>/dev/null; \
	fi; \
	cat test_stdout.tmp; \
	echo "========================================"; \
	echo "Verificare Memory Leaks ($(2)):"; \
	if command -v leaks > /dev/null 2>&1; then \
		MallocStackLogging=1 leaks --atExit -- ./$(1) > test_leaks.tmp 2>&1; \
		if grep -q "0 leaks" test_leaks.tmp; then \
			echo "  PASSED - Fara memory leaks!"; \
		else \
			echo "  FAILED - Memory leaks detectate!"; \
			grep "leaks for" test_leaks.tmp | sed 's/^/  /'; \
		fi; \
		rm -f test_leaks.tmp; \
	else \
		echo "  SKIPPED - leaks nu este disponibil"; \
	fi; \
	echo "========================================"; \
	rm -f test_stdout.tmp test_stderr.tmp
endef
else
define run_test
	@valgrind --leak-check=full ./$(1) > test_stdout.tmp 2> test_valgrind.tmp; \
	if grep -q "Process terminating with default action of signal" test_valgrind.tmp; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		echo ""; \
		grep "signal\|at 0x\|by 0x" test_valgrind.tmp | head -10 | sed 's/^==[0-9]*== /  /'; \
		echo ""; \
	fi; \
	cat test_stdout.tmp; \
	echo "========================================"; \
	echo "Verificare Valgrind ($(2)):"; \
	if grep -q "All heap blocks were freed -- no leaks are possible" test_valgrind.tmp; then \
		echo "  PASSED - Fara memory leaks!"; \
	else \
		echo "  FAILED - Memory leaks detectate!"; \
		grep "definitely lost\|indirectly lost\|possibly lost" test_valgrind.tmp | sed 's/^/  /'; \
	fi; \
	echo "========================================"; \
	rm -f test_stdout.tmp test_valgrind.tmp
endef
endif

test-stack: testStack
	$(call run_test,testStack,Stack)

test-queue: testQueue
	$(call run_test,testQueue,Queue)

test-parantheses: testParantheses
	$(call run_test,testParantheses,Parantheses)

test-radix: testRadixSort
	$(call run_test,testRadixSort,RadixSort)

test: $(TEST)
ifeq ($(UNAME_S),Darwin)
	@total=0; max=0; \
	for t in testStack testQueue testParantheses testRadixSort; do \
		if [ -f ./$$t ]; then \
			./$$t > test_stdout_$$t.tmp 2> test_stderr_$$t.tmp; \
			exit_code=$$?; \
			if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
				echo ""; \
				echo "  !! EROARE: $$t a primit SEGFAULT/ABORT !!"; \
				echo ""; \
			fi; \
			score=$$(cat test_stdout_$$t.tmp | grep "^SCOR" | grep -oE '[0-9]+\.[0-9]+' | head -1); \
			maxs=$$(cat test_stdout_$$t.tmp | grep "^SCOR" | grep -oE '[0-9]+\.[0-9]+' | tail -1); \
			cat test_stdout_$$t.tmp; \
			echo "========================================"; \
			tname=$$(echo $$t | sed 's/test//'); \
			echo "Verificare Memory Leaks ($$tname):"; \
			if command -v leaks > /dev/null 2>&1; then \
				MallocStackLogging=1 leaks --atExit -- ./$$t > test_leaks_$$t.tmp 2>&1; \
				if grep -q "0 leaks" test_leaks_$$t.tmp; then \
					echo "  PASSED - Fara memory leaks!"; \
				else \
					echo "  FAILED - Memory leaks detectate!"; \
				fi; \
				rm -f test_leaks_$$t.tmp; \
			else \
				echo "  SKIPPED - leaks nu este disponibil"; \
			fi; \
			echo "========================================"; \
			if [ -n "$$score" ]; then total=$$(echo "$$total + $$score" | bc); fi; \
			if [ -n "$$maxs" ]; then max=$$(echo "$$max + $$maxs" | bc); fi; \
			rm -f test_stdout_$$t.tmp test_stderr_$$t.tmp; \
		fi; \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "SCOR TOTAL: $$total / $$max"; \
	echo "========================================"
else
	@total=0; max=0; \
	for t in testStack testQueue testParantheses testRadixSort; do \
		if [ -f ./$$t ]; then \
			valgrind --leak-check=full ./$$t > test_stdout_$$t.tmp 2> test_valgrind_$$t.tmp; \
			if grep -q "Process terminating with default action of signal" test_valgrind_$$t.tmp; then \
				echo ""; \
				echo "  !! EROARE: $$t a primit SEGFAULT/ABORT !!"; \
				grep "signal\|at 0x\|by 0x" test_valgrind_$$t.tmp | head -10 | sed 's/^==[0-9]*== /  /'; \
				echo ""; \
			fi; \
			score=$$(cat test_stdout_$$t.tmp | grep "^SCOR" | grep -oE '[0-9]+\.[0-9]+' | head -1); \
			maxs=$$(cat test_stdout_$$t.tmp | grep "^SCOR" | grep -oE '[0-9]+\.[0-9]+' | tail -1); \
			cat test_stdout_$$t.tmp; \
			echo "========================================"; \
			tname=$$(echo $$t | sed 's/test//'); \
			echo "Verificare Valgrind ($$tname):"; \
			if grep -q "All heap blocks were freed -- no leaks are possible" test_valgrind_$$t.tmp; then \
				echo "  PASSED - Fara memory leaks!"; \
			else \
				echo "  FAILED - Memory leaks detectate!"; \
				grep "definitely lost\|indirectly lost\|possibly lost" test_valgrind_$$t.tmp | sed 's/^/  /'; \
			fi; \
			echo "========================================"; \
			if [ -n "$$score" ]; then total=$$(echo "$$total + $$score" | bc); fi; \
			if [ -n "$$maxs" ]; then max=$$(echo "$$max + $$maxs" | bc); fi; \
			rm -f test_stdout_$$t.tmp test_valgrind_$$t.tmp; \
		fi; \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "SCOR TOTAL: $$total / $$max"; \
	echo "========================================"
endif

$(EXEC):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@ $(LDFLAGS)

$(TEST):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC) $(TEST) test_stdout*.tmp test_valgrind*.tmp test_stderr*.tmp test_leaks*.tmp
