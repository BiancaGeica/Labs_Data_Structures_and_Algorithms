.PHONY: all build test clean valgrind

CC = gcc
CC_FLAGS = -std=c9x -g -O0 -Wall
LD_FLAGS = -lm

UNAME_S := $(shell uname -s)

all: TestHeap

build: TestHeap

TestHeap: TestHeap.c Heap.h
	$(CC) $(CC_FLAGS) TestHeap.c -o TestHeap $(LD_FLAGS)

ifeq ($(UNAME_S),Darwin)
test: TestHeap
	@./TestHeap > test_stdout.tmp 2> test_stderr.tmp; \
	exit_code=$$?; \
	SCORE=$$(grep '^SCORE:' test_stdout.tmp | cut -d: -f2); \
	if [ -z "$$SCORE" ]; then SCORE="0.00"; fi; \
	grep -v '^SCORE:\|^RUNNING:' test_stdout.tmp; \
	if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		CRASH=$$(grep '^RUNNING:' test_stdout.tmp | tail -1 | sed 's/^RUNNING://'); \
		if [ -n "$$CRASH" ]; then echo "  !! Crash la testul: $$CRASH !!"; fi; \
		echo ""; \
	fi; \
	echo "========================================"; \
	echo "Verificare Memory Leaks (freeQueue):"; \
	if command -v leaks > /dev/null 2>&1; then \
		MallocStackLogging=1 leaks --atExit -- ./TestHeap > test_leaks.tmp 2>&1; \
		if grep -q "0 leaks" test_leaks.tmp; then \
			echo "  PASSED - Fara memory leaks!"; \
			SCORE=$$(printf "%.2f" $$(echo "$$SCORE + 1.00" | bc)); \
			echo "freeQueue     \t- Toate testele au trecut!\tPuncte: 1.00"; \
		else \
			echo "  FAILED - Memory leaks detectate!"; \
			grep "leaks for" test_leaks.tmp | sed 's/^/  /'; \
			echo "freeQueue     \t- PICAT\t\t\t\tPuncte: 0.00"; \
		fi; \
		rm -f test_leaks.tmp; \
	else \
		echo "  SKIPPED - leaks nu este disponibil"; \
	fi; \
	echo "========================================"; \
	if [ "$$SCORE" = "10.00" ]; then \
		echo "Toate testele au trecut! Felicitari!"; \
	fi; \
	echo "Scor total: $$SCORE / 10.00"; \
	rm -f test_stdout.tmp test_stderr.tmp
else
test: TestHeap
	@./TestHeap > test_stdout.tmp 2> test_stderr.tmp; \
	exit_code=$$?; \
	SCORE=$$(grep '^SCORE:' test_stdout.tmp | cut -d: -f2); \
	if [ -z "$$SCORE" ]; then SCORE="0.00"; fi; \
	grep -v '^SCORE:\|^RUNNING:' test_stdout.tmp; \
	if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		CRASH=$$(grep '^RUNNING:' test_stdout.tmp | tail -1 | sed 's/^RUNNING://'); \
		if [ -n "$$CRASH" ]; then echo "  !! Crash la testul: $$CRASH !!"; fi; \
		echo ""; \
	fi; \
	echo "========================================"; \
	echo "Verificare Valgrind (freeQueue):"; \
	valgrind --leak-check=full ./TestHeap > test_valgrind_stdout.tmp 2> test_valgrind.tmp; \
	if grep -q "Process terminating with default action of signal" test_valgrind.tmp; then \
		echo "  !! SEGFAULT detectat in valgrind !!"; \
		grep "signal\|at 0x\|by 0x" test_valgrind.tmp | head -10 | sed 's/^==[0-9]*== /  /'; \
		echo ""; \
	fi; \
	if grep -q "^CRASHED:1" test_valgrind_stdout.tmp 2>/dev/null || grep -q "Process terminating with default action of signal" test_valgrind.tmp; then \
		echo "  FAILED - Programul a crashat, freeQueue nu poate fi verificat"; \
		echo "freeQueue     \t- PICAT\t\t\t\tPuncte: 0.00"; \
	elif grep -q "All heap blocks were freed -- no leaks are possible" test_valgrind.tmp; then \
		echo "  PASSED - Fara memory leaks!"; \
		SCORE=$$(printf "%.2f" $$(echo "$$SCORE + 1.00" | bc)); \
		echo "freeQueue     \t- Toate testele au trecut!\tPuncte: 1.00"; \
	else \
		echo "  FAILED - Memory leaks detectate!"; \
		grep "LEAK SUMMARY" -A 4 test_valgrind.tmp | sed 's/^==[0-9]*==/  /'; \
		echo "freeQueue     \t- PICAT\t\t\t\tPuncte: 0.00"; \
	fi; \
	if grep -q "Invalid read\|Invalid write\|uninitialised\|invalid free\|Conditional jump" test_valgrind.tmp; then \
		echo ""; \
		echo "  Erori de memorie detectate:"; \
		grep "Invalid read\|Invalid write\|uninitialised\|invalid free\|Conditional jump" test_valgrind.tmp | sort -u | head -5 | sed 's/^==[0-9]*==/  /'; \
	fi; \
	echo "========================================"; \
	if [ "$$SCORE" = "10.00" ]; then \
		echo "Toate testele au trecut! Felicitari!"; \
	fi; \
	echo "Scor total: $$SCORE / 10.00"; \
	rm -f test_stdout.tmp test_stderr.tmp test_valgrind.tmp test_valgrind_stdout.tmp
endif

valgrind: TestHeap
	valgrind --leak-check=full ./TestHeap

clean:
	rm -f TestHeap *.o *.png _tmp.dot test_*.tmp
