/*
*	Laboratorul 8 - Heap
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>

typedef int Item;
#include "Heap.h"

float total_score = 0;
int all_passed = 1;
int has_graphviz = 0;
char current_test[256] = "";

void crash_handler(int sig) {
	const char *sig_name = (sig == SIGSEGV) ? "SEGFAULT (SIGSEGV)" :
	                       (sig == SIGABRT) ? "ABORT (SIGABRT)" :
	                       (sig == SIGFPE)  ? "EROARE ARITMETICA (SIGFPE)" :
	                       "SEMNAL NECUNOSCUT";
	printf("\n\n");
	printf("  !! EROARE FATALA: %s !!\n", sig_name);
	if (current_test[0] != '\0')
		printf("  !! Programul s-a oprit in timpul testului: %s !!\n", current_test);
	printf("  !! Verificati accesul la memorie in implementare !!\n\n");
	printf("CRASHED:1\n");
	printf("SCORE:%.2f\n", total_score);
	_exit(128 + sig);
}

// ============================================================================
// Utilitare
// ============================================================================

int checkHeapProperty(APriQueue h) {
	if (h == NULL) return 0;
	for (long i = 0; i < h->size; i++) {
		long l = 2 * i + 1, r = 2 * i + 2;
		if (l < h->size && h->elem[l].prior > h->elem[i].prior) return 0;
		if (r < h->size && h->elem[r].prior > h->elem[i].prior) return 0;
	}
	return 1;
}

void printPriors(APriQueue h) {
	printf("[");
	for (long i = 0; i < h->size; i++) {
		if (i > 0) printf(", ");
		printf("%d", h->elem[i].prior);
	}
	printf("]");
}

ItemType makeItem(int p) {
	ItemType x;
	x.prior = p;
	x.data = p;
	return x;
}

void checker_free(APriQueue h) {
	if (h == NULL) return;
	if (h->elem != NULL) free(h->elem);
	free(h);
}

// ============================================================================
// GraphViz
// ============================================================================

void generatePNG(APriQueue h, const char *filename, const char *color) {
	if (!has_graphviz || !h || h->size == 0) return;
	FILE *f = fopen("_tmp.dot", "w");
	fprintf(f, "digraph Heap {\n");
	fprintf(f, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=%s];\n", color);
	for (long i = 0; i < h->size; i++) {
		fprintf(f, "    n%ld [label=\"%d\"];\n", i, h->elem[i].prior);
	}
	for (long i = 0; i < h->size; i++) {
		long l = 2 * i + 1, r = 2 * i + 2;
		if (l < h->size) fprintf(f, "    n%ld -> n%ld;\n", i, l);
		if (r < h->size) fprintf(f, "    n%ld -> n%ld;\n", i, r);
	}
	fprintf(f, "}\n");
	fclose(f);
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "dot _tmp.dot -Tpng -o %s 2>/dev/null", filename);
	system(cmd);
	remove("_tmp.dot");
}

void cleanPNGs() {
	system("rm -f _tmp.dot heap_*.png 2>/dev/null");
}

// ============================================================================
// Teste
// ============================================================================

int test_makeQueue() {
	int passed = 1;
	snprintf(current_test, sizeof(current_test), "makeQueue");
	printf("RUNNING:%s\n", current_test);

	APriQueue h = makeQueue(10);
	if (h == NULL) {
		printf("\n  makeQueue PICAT\n");
		printf("  makeQueue(10) a returnat NULL\n");
		passed = 0;
	} else if (h->elem == NULL) {
		printf("\n  makeQueue PICAT\n");
		printf("  Campul elem nu a fost alocat (este NULL)\n");
		passed = 0;
	} else if (h->maxHeapSize != 10) {
		printf("\n  makeQueue PICAT\n");
		printf("  maxHeapSize -> asteptat: 10, primit: %ld\n", h->maxHeapSize);
		passed = 0;
	} else if (h->size != 0) {
		printf("\n  makeQueue PICAT\n");
		printf("  size initial -> asteptat: 0, primit: %ld\n", h->size);
		passed = 0;
	}
	checker_free(h);

	if (passed) {
		total_score += 1.00;
		printf("makeQueue     \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("makeQueue     \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_getParent() {
	int passed = 1;
	snprintf(current_test, sizeof(current_test), "getParent");
	printf("RUNNING:%s\n", current_test);

	struct { int idx; int expected; } tests[] = {
		{1, 0}, {2, 0}, {3, 1}, {4, 1}, {5, 2}, {6, 2},
		{9, 4}, {10, 4}, {13, 6}, {14, 6}, {15, 7}
	};
	int n = sizeof(tests) / sizeof(tests[0]);

	for (int i = 0; i < n && passed; i++) {
		int got = getParent(tests[i].idx);
		if (got != tests[i].expected) {
			printf("\n  getParent PICAT\n");
			printf("  getParent(%d) -> asteptat: %d, primit: %d\n",
				tests[i].idx, tests[i].expected, got);
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.50;
		printf("getParent     \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("getParent     \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_getLeftChild() {
	int passed = 1;
	snprintf(current_test, sizeof(current_test), "getLeftChild");
	printf("RUNNING:%s\n", current_test);

	struct { int idx; int expected; } tests[] = {
		{0, 1}, {1, 3}, {2, 5}, {3, 7}, {4, 9},
		{5, 11}, {6, 13}, {7, 15}, {10, 21}
	};
	int n = sizeof(tests) / sizeof(tests[0]);

	for (int i = 0; i < n && passed; i++) {
		int got = getLeftChild(tests[i].idx);
		if (got != tests[i].expected) {
			printf("\n  getLeftChild PICAT\n");
			printf("  getLeftChild(%d) -> asteptat: %d, primit: %d\n",
				tests[i].idx, tests[i].expected, got);
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.50;
		printf("getLeftChild  \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("getLeftChild  \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_getRightChild() {
	int passed = 1;
	snprintf(current_test, sizeof(current_test), "getRightChild");
	printf("RUNNING:%s\n", current_test);

	struct { int idx; int expected; } tests[] = {
		{0, 2}, {1, 4}, {2, 6}, {3, 8}, {4, 10},
		{5, 12}, {6, 14}, {7, 16}, {10, 22}
	};
	int n = sizeof(tests) / sizeof(tests[0]);

	for (int i = 0; i < n && passed; i++) {
		int got = getRightChild(tests[i].idx);
		if (got != tests[i].expected) {
			printf("\n  getRightChild PICAT\n");
			printf("  getRightChild(%d) -> asteptat: %d, primit: %d\n",
				tests[i].idx, tests[i].expected, got);
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.50;
		printf("getRightChild \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("getRightChild \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_siftUp() {
	int passed = 1;
	APriQueue h = NULL;
	snprintf(current_test, sizeof(current_test), "siftUp");
	printf("RUNNING:%s\n", current_test);

	// Test 1: element deja in pozitie corecta (frunza mica) - nu trebuie sa urce
	h = makeQueue(10);
	h->elem[0] = makeItem(10);
	h->elem[1] = makeItem(8);
	h->elem[2] = makeItem(6);
	h->elem[3] = makeItem(5);
	h->size = 4;
	siftUp(h, 3);
	if (h->elem[0].prior != 10 || h->elem[3].prior != 5 ||
	    h->elem[1].prior != 8 || h->elem[2].prior != 6) {
		printf("\n  siftUp test1 PICAT (element mic pe frunza - nu trebuie sa urce)\n");
		printf("  Initial: [10, 8, 6, 5]\n");
		printf("  Dupa siftUp(3), asteptat: [10, 8, 6, 5], primit: ");
		printPriors(h); printf("\n");
		generatePNG(h, "heap_siftup_1.png", "orange");
		passed = 0;
	}
	checker_free(h);

	// Test 2: element mare la frunza - urca pana la radacina
	if (passed) {
		h = makeQueue(10);
		h->elem[0] = makeItem(5);
		h->elem[1] = makeItem(3);
		h->elem[2] = makeItem(4);
		h->elem[3] = makeItem(2);
		h->elem[4] = makeItem(10);
		h->size = 5;
		siftUp(h, 4);
		if (h->elem[0].prior != 10 || !checkHeapProperty(h)) {
			printf("\n  siftUp test2 PICAT (element mare urca pana la radacina)\n");
			printf("  Initial: [5, 3, 4, 2, 10]\n");
			printf("  Dupa siftUp(4), asteptat radacina: 10, primit: %d\n",
				h->elem[0].prior);
			printf("  Heap primit: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_siftup_2.png", "orange");
			passed = 0;
		}
		checker_free(h);
	}

	// Test 3: element urca doar un nivel si se opreste
	if (passed) {
		h = makeQueue(10);
		h->elem[0] = makeItem(20);
		h->elem[1] = makeItem(5);
		h->elem[2] = makeItem(18);
		h->elem[3] = makeItem(3);
		h->elem[4] = makeItem(10);
		h->size = 5;
		siftUp(h, 4);
		// 10 > 5 -> swap; 10 < 20 -> stop; rezultat: [20, 10, 18, 3, 5]
		if (h->elem[0].prior != 20 || h->elem[1].prior != 10 ||
		    h->elem[2].prior != 18 || h->elem[3].prior != 3 ||
		    h->elem[4].prior != 5) {
			printf("\n  siftUp test3 PICAT (element urca doar un nivel)\n");
			printf("  Initial: [20, 5, 18, 3, 10]\n");
			printf("  Dupa siftUp(4), asteptat: [20, 10, 18, 3, 5], primit: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_siftup_3.png", "orange");
			passed = 0;
		}
		checker_free(h);
	}

	if (passed) {
		total_score += 2.00;
		printf("siftUp        \t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		printf("siftUp        \t- PICAT\t\t\t\t(vezi heap_siftup_*.png)\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_insert() {
	int passed = 1;
	APriQueue h = NULL;
	snprintf(current_test, sizeof(current_test), "insert");
	printf("RUNNING:%s\n", current_test);

	// Test 1: insert si verificare proprietate + realocare
	h = makeQueue(4);
	int vals[] = {4, 2, 8, 7, 1, 9, 3};
	int n = 7;
	for (int i = 0; i < n; i++) insert(h, makeItem(vals[i]));

	if (h->size != n) {
		printf("\n  insert test1 PICAT: size incorect dupa %d inserari\n", n);
		printf("  Asteptat size=%d, primit size=%ld\n", n, h->size);
		generatePNG(h, "heap_insert_1.png", "orange");
		passed = 0;
	} else if (h->maxHeapSize < n) {
		printf("\n  insert test1 PICAT: capacitatea nu a fost marita la realocare\n");
		printf("  Dupa %d inserari, maxHeapSize=%ld (trebuie >= %d)\n",
			n, h->maxHeapSize, n);
		generatePNG(h, "heap_insert_1.png", "orange");
		passed = 0;
	} else if (!checkHeapProperty(h)) {
		printf("\n  insert test1 PICAT: proprietatea de max-heap nu este respectata\n");
		printf("  Inserate: {4, 2, 8, 7, 1, 9, 3}\n");
		printf("  Heap rezultat: ");
		printPriors(h); printf("\n");
		generatePNG(h, "heap_insert_1.png", "orange");
		passed = 0;
	} else if (h->elem[0].prior != 9) {
		printf("\n  insert test1 PICAT: radacina gresita\n");
		printf("  Valoarea maxima inserata: 9, dar radacina e: %d\n", h->elem[0].prior);
		generatePNG(h, "heap_insert_1.png", "orange");
		passed = 0;
	} else {
		generatePNG(h, "heap_insert_1.png", "green");
	}
	checker_free(h);

	// Test 2: insert secvential crescator (caz dificil pentru siftUp)
	if (passed) {
		h = makeQueue(2);
		for (int i = 1; i <= 10; i++) insert(h, makeItem(i));
		if (h->size != 10 || !checkHeapProperty(h) || h->elem[0].prior != 10) {
			printf("\n  insert test2 PICAT (inserare crescatoare 1..10)\n");
			printf("  Heap rezultat: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_insert_2.png", "orange");
			passed = 0;
		} else {
			generatePNG(h, "heap_insert_2.png", "green");
		}
		checker_free(h);
	}

	// Test 3: insert secvential descrescator (caz usor, nu declanseaza siftUp)
	if (passed) {
		h = makeQueue(2);
		for (int i = 10; i >= 1; i--) insert(h, makeItem(i));
		if (h->size != 10 || !checkHeapProperty(h) || h->elem[0].prior != 10) {
			printf("\n  insert test3 PICAT (inserare descrescatoare 10..1)\n");
			printf("  Heap rezultat: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_insert_3.png", "orange");
			passed = 0;
		} else {
			generatePNG(h, "heap_insert_3.png", "green");
		}
		checker_free(h);
	}

	if (passed) {
		total_score += 1.00;
		if (has_graphviz)
			printf("insert        \t- Toate testele au trecut!\tPuncte: 1.00\t(vezi heap_insert_*.png)\n");
		else
			printf("insert        \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("insert        \t- PICAT\t\t\t\t(vezi heap_insert_*.png)\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_getMax() {
	int passed = 1;
	APriQueue h = NULL;
	snprintf(current_test, sizeof(current_test), "getMax");
	printf("RUNNING:%s\n", current_test);

	// Test 1: heap cu mai multe elemente
	h = makeQueue(8);
	int vals[] = {3, 7, 1, 9, 5, 2, 8, 4};
	for (int i = 0; i < 8; i++) insert(h, makeItem(vals[i]));

	ItemType m = getMax(h);
	if (m.prior != 9) {
		printf("\n  getMax test1 PICAT\n");
		printf("  Inserate: {3, 7, 1, 9, 5, 2, 8, 4}\n");
		printf("  getMax().prior -> asteptat: 9, primit: %d\n", m.prior);
		passed = 0;
	}
	checker_free(h);

	// Test 2: heap cu un singur element
	if (passed) {
		h = makeQueue(1);
		insert(h, makeItem(42));
		m = getMax(h);
		if (m.prior != 42) {
			printf("\n  getMax test2 PICAT (heap cu un singur element)\n");
			printf("  getMax().prior -> asteptat: 42, primit: %d\n", m.prior);
			passed = 0;
		}
		checker_free(h);
	}

	if (passed) {
		total_score += 0.50;
		printf("getMax        \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("getMax        \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_siftDown() {
	int passed = 1;
	APriQueue h = NULL;
	snprintf(current_test, sizeof(current_test), "siftDown");
	printf("RUNNING:%s\n", current_test);

	// Test 1: radacina mica - coboara pana la pozitia corecta
	h = makeQueue(10);
	h->elem[0] = makeItem(1);
	h->elem[1] = makeItem(10);
	h->elem[2] = makeItem(8);
	h->elem[3] = makeItem(5);
	h->elem[4] = makeItem(7);
	h->size = 5;
	siftDown(h, 0);
	if (!checkHeapProperty(h) || h->elem[0].prior != 10) {
		printf("\n  siftDown test1 PICAT (radacina mica coboara)\n");
		printf("  Initial: [1, 10, 8, 5, 7]\n");
		printf("  Dupa siftDown(0), primit: ");
		printPriors(h); printf("\n");
		generatePNG(h, "heap_siftdown_1.png", "orange");
		passed = 0;
	}
	checker_free(h);

	// Test 2: heap deja valid - siftDown nu face nimic
	if (passed) {
		h = makeQueue(10);
		h->elem[0] = makeItem(20);
		h->elem[1] = makeItem(15);
		h->elem[2] = makeItem(10);
		h->elem[3] = makeItem(5);
		h->size = 4;
		siftDown(h, 0);
		if (h->elem[0].prior != 20 || h->elem[1].prior != 15 ||
		    h->elem[2].prior != 10 || h->elem[3].prior != 5) {
			printf("\n  siftDown test2 PICAT (heap deja valid - nu trebuie modificat)\n");
			printf("  Asteptat: [20, 15, 10, 5], primit: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_siftdown_2.png", "orange");
			passed = 0;
		}
		checker_free(h);
	}

	// Test 3: siftDown aplicat pe nod intern
	if (passed) {
		h = makeQueue(10);
		h->elem[0] = makeItem(50);
		h->elem[1] = makeItem(3);
		h->elem[2] = makeItem(40);
		h->elem[3] = makeItem(20);
		h->elem[4] = makeItem(15);
		h->size = 5;
		siftDown(h, 1);
		// 3 comparat cu max(20, 15)=20 -> swap; rezultat: [50, 20, 40, 3, 15]
		if (h->elem[0].prior != 50 || h->elem[1].prior != 20 ||
		    h->elem[2].prior != 40 || h->elem[3].prior != 3 ||
		    h->elem[4].prior != 15) {
			printf("\n  siftDown test3 PICAT (siftDown pe nod intern)\n");
			printf("  Initial: [50, 3, 40, 20, 15]\n");
			printf("  Dupa siftDown(1), asteptat: [50, 20, 40, 3, 15], primit: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_siftdown_3.png", "orange");
			passed = 0;
		}
		checker_free(h);
	}

	// Test 4: siftDown alege corect copilul mai mare
	if (passed) {
		h = makeQueue(10);
		h->elem[0] = makeItem(5);
		h->elem[1] = makeItem(20);
		h->elem[2] = makeItem(15);
		h->size = 3;
		siftDown(h, 0);
		// trebuie sa aleaga copilul stang (20) nu cel drept (15)
		if (h->elem[0].prior != 20 || h->elem[1].prior != 5) {
			printf("\n  siftDown test4 PICAT (alegere copil mai mare)\n");
			printf("  Initial: [5, 20, 15]\n");
			printf("  Dupa siftDown(0), asteptat: [20, 5, 15], primit: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_siftdown_4.png", "orange");
			passed = 0;
		}
		checker_free(h);
	}

	if (passed) {
		total_score += 2.00;
		printf("siftDown      \t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		printf("siftDown      \t- PICAT\t\t\t\t(vezi heap_siftdown_*.png)\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_removeMax() {
	int passed = 1;
	APriQueue h = NULL;
	snprintf(current_test, sizeof(current_test), "removeMax");
	printf("RUNNING:%s\n", current_test);

	// Test 1: removeMax repetat - extractiile trebuie sa fie in ordine descrescatoare
	h = makeQueue(4);
	int vals[] = {4, 2, 8, 7, 1, 9, 3, 6, 5};
	int n = 9;
	for (int i = 0; i < n; i++) insert(h, makeItem(vals[i]));

	int expected[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
	for (int i = 0; i < n && passed; i++) {
		int max = getMax(h).prior;
		if (max != expected[i]) {
			printf("\n  removeMax PICAT: la pasul %d\n", i + 1);
			printf("  Asteptat max: %d, primit: %d\n", expected[i], max);
			printf("  Heap curent: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_removemax.png", "orange");
			passed = 0;
			break;
		}
		removeMax(h);
		if (h->size != n - i - 1) {
			printf("\n  removeMax PICAT: size gresit dupa removeMax #%d\n", i + 1);
			printf("  Asteptat: %d, primit: %ld\n", n - i - 1, h->size);
			passed = 0;
			break;
		}
		if (h->size > 0 && !checkHeapProperty(h)) {
			printf("\n  removeMax PICAT: proprietatea de heap incalcata dupa removeMax #%d\n", i + 1);
			printf("  Heap: ");
			printPriors(h); printf("\n");
			generatePNG(h, "heap_removemax.png", "orange");
			passed = 0;
			break;
		}
	}
	checker_free(h);

	if (passed) {
		total_score += 1.00;
		printf("removeMax     \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("removeMax     \t- PICAT\t\t\t\t(vezi heap_removemax.png)\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

void run_all() {
	test_makeQueue();
	test_getParent();
	test_getLeftChild();
	test_getRightChild();
	test_siftUp();
	test_insert();
	test_getMax();
	test_siftDown();
	test_removeMax();
}

void run_selective(int argc, char **argv) {
	int viz[10] = {0};
	for (int i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		if (n < 1 || n > 9 || viz[n]) continue;
		viz[n] = 1;
		switch (n) {
			case 1: test_makeQueue(); break;
			case 2: test_getParent(); break;
			case 3: test_getLeftChild(); break;
			case 4: test_getRightChild(); break;
			case 5: test_siftUp(); break;
			case 6: test_insert(); break;
			case 7: test_getMax(); break;
			case 8: test_siftDown(); break;
			case 9: test_removeMax(); break;
		}
	}
}

void test_freeQueue_check() {
	APriQueue h = makeQueue(3);
	if (h == NULL) return;
	insert(h, makeItem(10));
	insert(h, makeItem(20));
	insert(h, makeItem(5));
	insert(h, makeItem(30));
	freeQueue(h);
}

int main(int argc, char **argv) {
	signal(SIGSEGV, crash_handler);
	signal(SIGABRT, crash_handler);
	signal(SIGFPE, crash_handler);
	setvbuf(stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	int exit_code = system("which dot > /dev/null 2>&1");
	has_graphviz = (WEXITSTATUS(exit_code) == 0);
	if (!has_graphviz) {
		printf("(!) graphviz nu este instalat - nu se vor genera imagini cu heap-urile\n");
		printf("    Instalare: sudo apt install graphviz\n\n");
	}
	cleanPNGs();

	if (argc == 1)
		run_all();
	else
		run_selective(argc, argv);

	// Test final pentru freeQueue (verificat via valgrind)
	test_freeQueue_check();

	printf("SCORE:%.2f\n", total_score);
	return all_passed ? 0 : 1;
}
