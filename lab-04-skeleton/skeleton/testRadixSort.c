#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_LEN 256

typedef int Item;
#include "Queue.h"
#include "radix_sort.h"

// -----------------------------------------------------------------------------
// Macros (stil lab03)
// -----------------------------------------------------------------------------

#define sd_assert(message, test) \
	do { \
		if (!(test)) \
			return message; \
	} while (0)

#define sd_run_test(test, score) \
	do { \
		char *message = test(); \
		tests_run++; \
		if (message) { \
			printf("%s\n", message); \
			return message; \
		} else { \
			total_score += score; \
			printf("\tPuncte: %.2f\n", score); \
		} \
	} while (0)

int tests_run = 0;
float total_score = 0;

// Functie auxiliara: verifica daca array-ul e sortat crescator
int isSorted(int *arr, int len) {
	for (int i = 1; i < len; i++) {
		if (arr[i] < arr[i - 1]) return 0;
	}
	return 1;
}

// Functie auxiliara: verifica daca doua array-uri contin aceleasi elemente
int sameElements(int *a, int *aCopy, int len) {
	// sortam copia cu qsort pentru comparare
	int *tmp = (int *)malloc(len * sizeof(int));
	memcpy(tmp, a, len * sizeof(int));
	// bubble sort pe tmp (e mic)
	for (int i = 0; i < len - 1; i++)
		for (int j = 0; j < len - i - 1; j++)
			if (tmp[j] > tmp[j + 1]) {
				int t = tmp[j]; tmp[j] = tmp[j + 1]; tmp[j + 1] = t;
			}

	int *tmp2 = (int *)malloc(len * sizeof(int));
	memcpy(tmp2, aCopy, len * sizeof(int));
	for (int i = 0; i < len - 1; i++)
		for (int j = 0; j < len - i - 1; j++)
			if (tmp2[j] > tmp2[j + 1]) {
				int t = tmp2[j]; tmp2[j] = tmp2[j + 1]; tmp2[j + 1] = t;
			}

	int ok = 1;
	for (int i = 0; i < len; i++)
		if (tmp[i] != tmp2[i]) { ok = 0; break; }
	free(tmp);
	free(tmp2);
	return ok;
}

void printArray(int *arr, int len) {
	printf("[");
	for (int i = 0; i < len; i++) {
		printf("%d", arr[i]);
		if (i < len - 1) printf(", ");
	}
	printf("]");
}

// =============================================================================
// Test: array simplu
// =============================================================================
static char *test_simple() {
	int arr[] = {3, 1, 2};
	int copy[] = {3, 1, 2};
	int len = 3;

	radixSort(arr, len);
	if (!isSorted(arr, len)) {
		printf("\n  radixSort([3,1,2]): rezultat nu e sortat: ");
		printArray(arr, len); printf("\n");
	}
	sd_assert("simple - Test 1 picat (array [3,1,2] nu e sortat)", isSorted(arr, len));
	sd_assert("simple - Test 2 picat (elemente modificate)", sameElements(arr, copy, len));

	// Array deja sortat
	int arr2[] = {1, 2, 3, 4, 5};
	int copy2[] = {1, 2, 3, 4, 5};
	radixSort(arr2, 5);
	sd_assert("simple - Test 3 picat (array deja sortat)", isSorted(arr2, 5));
	sd_assert("simple - Test 4 picat (elemente modificate)", sameElements(arr2, copy2, 5));

	// Array cu un element
	int arr3[] = {42};
	radixSort(arr3, 1);
	sd_assert("simple - Test 5 picat (1 element)", arr3[0] == 42);

	// Array cu elemente egale
	int arr4[] = {5, 5, 5, 5};
	radixSort(arr4, 4);
	sd_assert("simple - Test 6 picat (elemente egale)", isSorted(arr4, 4));

	printf("simple \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: array din fisierul CSV (linia 1)
// =============================================================================
static char *test_csv1() {
	int arr[] = {0, 32, 1, 499, 657, 2, 12, 80, 27, 512, 45, 20, 32, 15, 54, 9, 41, 8};
	int copy[] = {0, 32, 1, 499, 657, 2, 12, 80, 27, 512, 45, 20, 32, 15, 54, 9, 41, 8};
	int len = 18;

	radixSort(arr, len);
	if (!isSorted(arr, len)) {
		printf("\n  radixSort(linia 1 CSV): rezultat nu e sortat: ");
		printArray(arr, len); printf("\n");
	}
	sd_assert("csv1 - Test 1 picat (nu e sortat)", isSorted(arr, len));
	sd_assert("csv1 - Test 2 picat (elemente modificate)", sameElements(arr, copy, len));

	// Verificam cateva pozitii
	sd_assert("csv1 - Test 3 picat (primul element != 0)", arr[0] == 0);
	sd_assert("csv1 - Test 4 picat (ultimul element != 657)", arr[len - 1] == 657);

	printf("csv1 \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: array din fisierul CSV (linia 2 - numere mari)
// =============================================================================
static char *test_csv2() {
	int arr[] = {51023, 4341, 1224, 190, 564, 76, 23, 478, 589, 5, 23, 67, 20123};
	int copy[] = {51023, 4341, 1224, 190, 564, 76, 23, 478, 589, 5, 23, 67, 20123};
	int len = 13;

	radixSort(arr, len);
	if (!isSorted(arr, len)) {
		printf("\n  radixSort(linia 2 CSV): rezultat nu e sortat: ");
		printArray(arr, len); printf("\n");
	}
	sd_assert("csv2 - Test 1 picat (nu e sortat)", isSorted(arr, len));
	sd_assert("csv2 - Test 2 picat (elemente modificate)", sameElements(arr, copy, len));
	sd_assert("csv2 - Test 3 picat (primul element != 5)", arr[0] == 5);
	sd_assert("csv2 - Test 4 picat (ultimul element != 51023)", arr[len - 1] == 51023);

	printf("csv2 \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: array din fisierul CSV (linia 3) + cu zero
// =============================================================================
static char *test_csv3() {
	int arr[] = {5689, 237, 2347, 789, 0, 1289, 78, 5, 23, 4, 712, 7238, 897, 453};
	int copy[] = {5689, 237, 2347, 789, 0, 1289, 78, 5, 23, 4, 712, 7238, 897, 453};
	int len = 14;

	radixSort(arr, len);
	if (!isSorted(arr, len)) {
		printf("\n  radixSort(linia 3 CSV): rezultat nu e sortat: ");
		printArray(arr, len); printf("\n");
	}
	sd_assert("csv3 - Test 1 picat (nu e sortat)", isSorted(arr, len));
	sd_assert("csv3 - Test 2 picat (elemente modificate)", sameElements(arr, copy, len));
	sd_assert("csv3 - Test 3 picat (primul element != 0)", arr[0] == 0);
	sd_assert("csv3 - Test 4 picat (ultimul element != 7238)", arr[len - 1] == 7238);

	printf("csv3 \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Main
// =============================================================================
static char *all_tests() {
	sd_run_test(test_simple, 0.50);
	sd_run_test(test_csv1,   0.50);
	sd_run_test(test_csv2,   0.50);
	sd_run_test(test_csv3,   0.50);
	return 0;
}

int main(void) {
	char *result = all_tests();

	printf("\n");
	if (result != 0) {
		printf("ESUAT: %s\n", result);
	}
	printf("Teste rulate: %d / %d\n", tests_run, 4);
	printf("SCOR RadixSort (Bonus): %.2f / 2.00\n\n", total_score);

	return result != 0;
}
