CC = gcc
CC_FLAGS = -g -Wall

UNAME_S := $(shell uname -s)

all: testTree

tree.o: tree.c tree.h
	$(CC) -c tree.c $(CC_FLAGS)

testTree: tree.o main.c tree.h
	$(CC) main.c tree.o -o testTree $(CC_FLAGS)

ifeq ($(UNAME_S),Darwin)
test: testTree
	@./testTree > test_stdout.tmp 2> test_stderr.tmp; \
	exit_code=$$?; \
	if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		echo ""; \
		cat test_stderr.tmp 2>/dev/null; \
	fi; \
	SCORE=$$(grep '^SCORE:' test_stdout.tmp | cut -d: -f2); \
	grep -v '^SCORE:' test_stdout.tmp; \
	echo "========================================"; \
	echo "Verificare Memory Leaks (freeTree):"; \
	if command -v leaks > /dev/null 2>&1; then \
		MallocStackLogging=1 leaks --atExit -- ./testTree > test_leaks.tmp 2>&1; \
		if grep -q "0 leaks" test_leaks.tmp; then \
			echo "  PASSED - Fara memory leaks!"; \
			SCORE=$$(echo "$$SCORE + 0.75" | bc); \
			echo "freeTree \t\t\t\tPuncte: 0.75"; \
		else \
			echo "  FAILED - Memory leaks detectate!"; \
			grep "leaks for" test_leaks.tmp | sed 's/^/  /'; \
			echo "freeTree \t\t\t\tPuncte: 0.00"; \
		fi; \
		rm -f test_leaks.tmp; \
	else \
		echo "  SKIPPED - leaks nu este disponibil"; \
	fi; \
	echo "========================================"; \
	if [ "$$SCORE" = "10.00" ]; then \
		echo "Toate testele au trecut! Felicitari!"; \
	fi; \
	echo "Punctajul obtinut este: $$SCORE"; \
	rm -f test_stdout.tmp test_stderr.tmp
else
test: testTree
	@./testTree > test_stdout.tmp 2> test_stderr.tmp; \
	exit_code=$$?; \
	if [ $$exit_code -eq 139 ] || [ $$exit_code -eq 134 ] || [ $$exit_code -eq 136 ] || [ $$exit_code -eq 11 ]; then \
		echo ""; \
		echo "  !! EROARE: Programul a primit SEGFAULT/ABORT !!"; \
		echo "  !! Verificati accesul la memorie in implementare !!"; \
		echo ""; \
		cat test_stderr.tmp 2>/dev/null; \
	fi; \
	SCORE=$$(grep '^SCORE:' test_stdout.tmp | cut -d: -f2); \
	grep -v '^SCORE:' test_stdout.tmp; \
	echo "========================================"; \
	echo "Verificare Valgrind (freeTree):"; \
	valgrind --leak-check=full ./testTree > /dev/null 2> test_valgrind.tmp; \
	if grep -q "Process terminating with default action of signal" test_valgrind.tmp; then \
		echo "  !! SEGFAULT detectat in valgrind !!"; \
		grep "signal\|at 0x\|by 0x" test_valgrind.tmp | head -10 | sed 's/^==[0-9]*== /  /'; \
		echo ""; \
	fi; \
	if grep -q "All heap blocks were freed -- no leaks are possible" test_valgrind.tmp; then \
		echo "  PASSED - Fara memory leaks!"; \
		SCORE=$$(echo "$$SCORE + 0.75" | bc); \
		echo "freeTree \t\t\t\tPuncte: 0.75"; \
	else \
		echo "  FAILED - Memory leaks detectate!"; \
		grep "LEAK SUMMARY" -A 4 test_valgrind.tmp | sed 's/^==[0-9]*==/  /'; \
		echo "freeTree \t\t\t\tPuncte: 0.00"; \
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
	echo "Punctajul obtinut este: $$SCORE"; \
	rm -f test_stdout.tmp test_stderr.tmp test_valgrind.tmp
endif

valgrind: testTree
	valgrind --leak-check=full ./testTree

clean:
	rm -f *.o *~ testTree *.dot *.png test_*.tmp
