#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 255

typedef char Item;

#include "DoubleLinkedList.h"

// -----------------------------------------------------------------------------
// Macros
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

// -----------------------------------------------------------------------------
// Functii auxiliare pentru teste
// -----------------------------------------------------------------------------

List* manualBuild(const char *str) {
	List *list = createList();
	int len = strlen(str);
	if (len == 0) return list;

	ListNode *prev = NULL;
	for (int i = 0; i < len; i++) {
		ListNode *node = createNode(str[i]);
		node->prev = prev;
		if (prev != NULL)
			prev->next = node;
		else
			list->first = node;
		prev = node;
	}
	list->last = prev;
	return list;
}

// Elibereaza memoria unei liste
void manualFree(List *list) {
	if (list == NULL) return;
	ListNode *curr = list->first;
	while (curr != NULL) {
		ListNode *next = curr->next;
		free(curr);
		curr = next;
	}
	free(list);
}

// Numara nodurile unei liste
int manualLength(List *list) {
	int count = 0;
	ListNode *curr = list->first;
	while (curr != NULL) {
		count++;
		curr = curr->next;
	}
	return count;
}

// Verifica daca lista contine elementele asteptate
int manualCheck(List *list, const char *expected) {
	int len = strlen(expected);
	if (len == 0)
		return (list->first == NULL && list->last == NULL);
	ListNode *it = list->first;
	for (int i = 0; i < len; i++) {
		if (it == NULL) return 0;
		if (it->elem != expected[i]) return 0;
		it = it->next;
	}
	return (it == NULL);
}

int verifyLinks(List *list) {
	if (list == NULL) return 1;
	if (list->first == NULL && list->last == NULL) return 1;
	if (list->first == NULL || list->last == NULL) return 0;
	if (list->first->prev != NULL) return 0;
	if (list->last->next != NULL) return 0;
	ListNode *it = list->first;
	while (it->next != NULL) {
		if (it->next->prev != it) return 0;
		it = it->next;
	}
	if (it != list->last) return 0;
	return 1;
}

void printList(List *list) {
	if (list == NULL || list->first == NULL) {
		printf("[]");
		return;
	}
	ListNode *it = list->first;
	printf("[");
	while (it != NULL) {
		printf("%c", it->elem);
		if (it->next != NULL) printf(", ");
		it = it->next;
	}
	printf("]");
}

void printExpected(const char *expected) {
	int len = strlen(expected);
	if (len == 0) { printf("[]"); return; }
	printf("[");
	for (int i = 0; i < len; i++) {
		if (i > 0) printf(", ");
		printf("%c", expected[i]);
	}
	printf("]");
}

void showMismatch(List *list, const char *expected, const char *operation) {
	printf("\n  Operatie: %s", operation);
	printf("\n  Asteptat: "); printExpected(expected);
	printf("\n  Primit:   "); printList(list);
	printf("\n");
}

// =============================================================================
// Test: createList + createNode
// =============================================================================
static char *test_create() {
	char *err = NULL;
	List *list = createList();
	if (list == NULL) {
		printf("\n  createList() a returnat NULL\n");
		return "create - Test 1 picat (createList returneaza NULL)";
	}
	if (list->first != NULL)
		printf("\n  createList()->first != NULL (asteptat: NULL)\n");
	if (list->first != NULL) { err = "create - Test 2 picat (first != NULL)"; }
	if (!err && list->last != NULL) {
		printf("\n  createList()->last != NULL (asteptat: NULL)\n");
		err = "create - Test 3 picat (last != NULL)";
	}
	free(list);
	if (err) return err;

	ListNode *node = createNode('a');
	if (node == NULL) {
		printf("\n  createNode('a') a returnat NULL\n");
		return "create - Test 4 picat (createNode returneaza NULL)";
	}
	if (node->elem != 'a')
		printf("\n  createNode('a')->elem: asteptat 'a', primit '%c'\n", node->elem);
	if (node->elem != 'a') { err = "create - Test 5 picat (elem incorect)"; }
	if (!err && node->next != NULL) {
		printf("\n  createNode('a')->next != NULL\n");
		err = "create - Test 6 picat (next != NULL)";
	}
	if (!err && node->prev != NULL) {
		printf("\n  createNode('a')->prev != NULL\n");
		err = "create - Test 7 picat (prev != NULL)";
	}
	free(node);
	if (err) return err;

	node = createNode('Z');
	if (node->elem != 'Z')
		printf("\n  createNode('Z')->elem: asteptat 'Z', primit '%c'\n", node->elem);
	int zOk = (node->elem == 'Z');
	free(node);
	sd_assert("create - Test 8 picat (createNode('Z') elem incorect)", zOk);

	printf("create \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: isEmpty
// =============================================================================
static char *test_isEmpty() {
	char *err = NULL;
	int res;

	// NULL
	res = isEmpty(NULL);
	if (res != 1)
		printf("\n  isEmpty(NULL): asteptat 1, primit %d\n", res);
	sd_assert("isEmpty - Test 1 picat (NULL)", res == 1);

	// Lista goala (alocata dar fara noduri)
	List *list = createList();
	ListNode *node = createNode('x');
	ListNode *node2 = createNode('y');

	res = isEmpty(list);
	if (res != 1)
		printf("\n  isEmpty(lista goala): asteptat 1, primit %d\n", res);
	if (res != 1) { err = "isEmpty - Test 2 picat (lista goala)"; goto cleanup_empty; }

	// Lista cu 1 nod (construit manual)
	list->first = node;
	list->last = node;
	res = isEmpty(list);
	if (res != 0)
		printf("\n  isEmpty(lista cu 1 nod): asteptat 0, primit %d\n", res);
	if (res != 0) { err = "isEmpty - Test 3 picat (lista cu 1 nod)"; goto cleanup_empty; }

	// Lista cu 2 noduri
	node->next = node2;
	node2->prev = node;
	list->last = node2;
	res = isEmpty(list);
	if (res != 0)
		printf("\n  isEmpty(lista cu 2 noduri): asteptat 0, primit %d\n", res);
	if (res != 0) { err = "isEmpty - Test 4 picat (lista cu 2 noduri)"; goto cleanup_empty; }

	// Golim manual
	list->first = NULL;
	list->last = NULL;
	res = isEmpty(list);
	if (res != 1)
		printf("\n  isEmpty(dupa golire manuala): asteptat 1, primit %d\n", res);
	if (res != 1) { err = "isEmpty - Test 5 picat (dupa golire manuala)"; goto cleanup_empty; }

cleanup_empty:
	free(node);
	free(node2);
	free(list);
	if (err) return err;
	printf("isEmpty \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: contains
// =============================================================================
static char *test_contains() {
	int res;

	// Lista goala
	List *list = createList();
	res = contains(list, 'x');
	free(list);
	if (res != 0)
		printf("\n  contains(lista goala, 'x'): asteptat 0, primit %d\n", res);
	sd_assert("contains - Test 1 picat (lista goala)", res == 0);

	// Lista cu 1 element
	list = manualBuild("a");
	int r1 = contains(list, 'a');
	int r2 = contains(list, 'b');
	manualFree(list);
	if (r1 != 1) printf("\n  contains(['a'], 'a'): asteptat 1, primit 0\n");
	sd_assert("contains - Test 2 picat ('a' in ['a'])", r1 == 1);
	if (r2 != 0) printf("\n  contains(['a'], 'b'): asteptat 0, primit 1\n");
	sd_assert("contains - Test 3 picat ('b' in ['a'])", r2 == 0);

	// Lista cu mai multe elemente: [a, b, c, d]
	list = manualBuild("abcd");
	int ra = contains(list, 'a');
	int rb = contains(list, 'b');
	int rc = contains(list, 'c');
	int rd = contains(list, 'd');
	int rz = contains(list, 'z');
	int rm = contains(list, '-');
	manualFree(list);

	if (ra != 1) printf("\n  contains([a,b,c,d], 'a' - first): asteptat 1, primit 0\n");
	sd_assert("contains - Test 4 picat (first 'a')", ra == 1);
	if (rb != 1) printf("\n  contains([a,b,c,d], 'b' - middle): asteptat 1, primit 0\n");
	sd_assert("contains - Test 5 picat (middle 'b')", rb == 1);
	if (rc != 1) printf("\n  contains([a,b,c,d], 'c' - middle): asteptat 1, primit 0\n");
	sd_assert("contains - Test 6 picat (middle 'c')", rc == 1);
	if (rd != 1) printf("\n  contains([a,b,c,d], 'd' - last): asteptat 1, primit 0\n");
	sd_assert("contains - Test 7 picat (last 'd')", rd == 1);
	if (rz != 0) printf("\n  contains([a,b,c,d], 'z'): asteptat 0, primit 1\n");
	sd_assert("contains - Test 8 picat ('z' inexistent)", rz == 0);
	if (rm != 0) printf("\n  contains([a,b,c,d], '-'): asteptat 0, primit 1\n");
	sd_assert("contains - Test 9 picat ('-' inexistent)", rm == 0);

	printf("contains \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: insertAt
// =============================================================================
static char *test_insertAt() {
	char *err = NULL;
	List *list = createList();

	// --- Pozitii invalide pe lista goala ---
	insertAt(list, 'x', -1);
	if (list->first != NULL) {
		printf("\n  insertAt([], 'x', -1): lista trebuie sa ramana goala\n");
		printf("  Primit: "); printList(list); printf("\n");
	}
	if (list->first != NULL) { err = "insertAt - Test 1 picat (pos -1 pe lista goala)"; goto cleanup_insert; }

	insertAt(list, 'x', 1);
	if (list->first != NULL) { err = "insertAt - Test 2 picat (pos 1 pe lista goala)"; goto cleanup_insert; }

	insertAt(list, 'x', 5);
	if (list->first != NULL) { err = "insertAt - Test 3 picat (pos 5 pe lista goala)"; goto cleanup_insert; }

	// --- Insert in lista goala la pos 0 ---
	insertAt(list, 'c', 0);
	if (list->first == NULL) {
		printf("\n  insertAt([], 'c', 0): lista trebuie sa aiba 1 element, dar first e NULL\n");
		err = "insertAt - Test 4 picat (insert in lista goala, first e NULL)"; goto cleanup_insert;
	}
	if (list->first != list->last) {
		printf("\n  insertAt([], 'c', 0): first != last (1 singur element)\n");
		err = "insertAt - Test 5 picat (first != last pe 1 elem)"; goto cleanup_insert;
	}
	if (list->first->elem != 'c') {
		printf("\n  insertAt([], 'c', 0): elem asteptat 'c' (=%d), primit '%c' (=%d)\n",
			'c', list->first->elem, list->first->elem);
		err = "insertAt - Test 6 picat (elem != 'c')"; goto cleanup_insert;
	}
	if (list->first->prev != NULL) { err = "insertAt - Test 7 picat (first->prev != NULL)"; goto cleanup_insert; }
	if (list->last->next != NULL) { err = "insertAt - Test 8 picat (last->next != NULL)"; goto cleanup_insert; }

	// --- Pozitii invalide pe lista cu 1 elem ---
	insertAt(list, 'x', -1);
	if (manualLength(list) != 1) {
		printf("\n  insertAt(['c'], 'x', -1): length asteptat 1, primit %d\n", manualLength(list));
		err = "insertAt - Test 9 picat (pos -1 pe 1 elem)"; goto cleanup_insert;
	}

	insertAt(list, 'x', 2);
	if (manualLength(list) != 1) { err = "insertAt - Test 10 picat (pos 2 pe 1 elem)"; goto cleanup_insert; }

	insertAt(list, 'x', 100);
	if (manualLength(list) != 1) { err = "insertAt - Test 11 picat (pos 100 pe 1 elem)"; goto cleanup_insert; }

	// --- Insert la inceput (pos 0) ---
	insertAt(list, 'a', 0);
	if (!manualCheck(list, "ac")) {
		showMismatch(list, "ac", "insertAt(['c'], 'a', 0)");
		err = "insertAt - Test 12 picat (insert la inceput)"; goto cleanup_insert;
	}
	if (list->first->elem != 'a') { err = "insertAt - Test 13 picat (first != 'a')"; goto cleanup_insert; }
	if (list->last->elem != 'c') { err = "insertAt - Test 14 picat (last != 'c')"; goto cleanup_insert; }
	if (list->first->prev != NULL) { err = "insertAt - Test 15 picat (first->prev != NULL)"; goto cleanup_insert; }
	if (list->last->next != NULL) { err = "insertAt - Test 16 picat (last->next != NULL)"; goto cleanup_insert; }
	if (!verifyLinks(list)) {
		printf("\n  Legaturi prev/next incorecte dupa insertAt(['c'], 'a', 0)\n");
		err = "insertAt - Test 17 picat (links incorecte)"; goto cleanup_insert;
	}

	// --- Insert la sfarsit (pos == manualLength) ---
	insertAt(list, 'e', 2);
	if (!manualCheck(list, "ace")) {
		showMismatch(list, "ace", "insertAt(['a','c'], 'e', 2)");
		err = "insertAt - Test 18 picat (insert la sfarsit)"; goto cleanup_insert;
	}
	if (list->last->elem != 'e') { err = "insertAt - Test 19 picat (last != 'e')"; goto cleanup_insert; }
	if (!verifyLinks(list)) { err = "insertAt - Test 20 picat (links dupa insert sfarsit)"; goto cleanup_insert; }

	// --- Insert la mijloc (pos 1) ---
	insertAt(list, 'b', 1);
	if (!manualCheck(list, "abce")) {
		showMismatch(list, "abce", "insertAt(['a','c','e'], 'b', 1)");
		err = "insertAt - Test 21 picat (insert la mijloc pos 1)"; goto cleanup_insert;
	}
	if (!verifyLinks(list)) { err = "insertAt - Test 22 picat (links dupa insert mijloc)"; goto cleanup_insert; }

	// --- Insert la mijloc (pos 3) ---
	insertAt(list, 'd', 3);
	if (!manualCheck(list, "abcde")) {
		showMismatch(list, "abcde", "insertAt(['a','b','c','e'], 'd', 3)");
		err = "insertAt - Test 23 picat (insert la mijloc pos 3)"; goto cleanup_insert;
	}
	if (!verifyLinks(list)) { err = "insertAt - Test 24 picat (links dupa insert mijloc 3)"; goto cleanup_insert; }

	if (manualLength(list) != 5) {
		printf("\n  length dupa 5 inserari: asteptat 5, primit %d\n", manualLength(list));
		err = "insertAt - Test 25 picat (length != 5)"; goto cleanup_insert;
	}
	if (list->first->elem != 'a') { err = "insertAt - Test 26 picat (first != 'a')"; goto cleanup_insert; }
	if (list->last->elem != 'e') { err = "insertAt - Test 27 picat (last != 'e')"; goto cleanup_insert; }

	// --- Insert la 0 pe lista mare ---
	insertAt(list, 'Z', 0);
	if (!manualCheck(list, "Zabcde")) {
		showMismatch(list, "Zabcde", "insertAt(['a'..'e'], 'Z', 0)");
		err = "insertAt - Test 28 picat (insert 0 pe lista mare)"; goto cleanup_insert;
	}
	if (list->first->elem != 'Z') { err = "insertAt - Test 29 picat (first != 'Z')"; goto cleanup_insert; }
	if (!verifyLinks(list)) { err = "insertAt - Test 30 picat (links)"; goto cleanup_insert; }

	// --- Insert la sfarsit pe lista mare ---
	insertAt(list, 'W', manualLength(list));
	if (!manualCheck(list, "ZabcdeW")) {
		showMismatch(list, "ZabcdeW", "insertAt([Z,a..e], 'W', 6)");
		err = "insertAt - Test 31 picat (insert sfarsit lista mare)"; goto cleanup_insert;
	}
	if (list->last->elem != 'W') { err = "insertAt - Test 32 picat (last != 'W')"; goto cleanup_insert; }
	if (!verifyLinks(list)) { err = "insertAt - Test 33 picat (links sfarsit)"; goto cleanup_insert; }

cleanup_insert:
	manualFree(list);
	if (err) return err;
	printf("insertAt \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: deleteOnce
// =============================================================================
static char *test_deleteOnce() {
	char *err = NULL;

	// --- Delete din lista goala ---
	List *list = createList();
	deleteOnce(list, 'x');
	int emptyOk = (list->first == NULL && list->last == NULL);
	free(list);
	if (!emptyOk) printf("\n  deleteOnce([], 'x'): lista trebuie sa ramana goala\n");
	sd_assert("deleteOnce - Test 1 picat (delete din lista goala)", emptyOk);

	// --- Delete element inexistent ---
	list = manualBuild("ab");
	deleteOnce(list, 'z');
	int lenOk = (manualLength(list) == 2);
	int contentOk = manualCheck(list, "ab");
	int linksOk = verifyLinks(list);
	if (!lenOk)
		printf("\n  deleteOnce(['a','b'], 'z'): length asteptat 2, primit %d\n", manualLength(list));
	if (!contentOk) showMismatch(list, "ab", "deleteOnce(['a','b'], 'z')");
	manualFree(list);
	sd_assert("deleteOnce - Test 2 picat (element inexistent)", lenOk);
	sd_assert("deleteOnce - Test 3 picat (continut dupa delete inexistent)", contentOk);
	sd_assert("deleteOnce - Test 4 picat (links dupa delete inexistent)", linksOk);

	// --- Delete nod unic ---
	list = manualBuild("x");
	deleteOnce(list, 'x');
	if (list->first != NULL || list->last != NULL) {
		printf("\n  deleteOnce(['x'], 'x'): lista trebuie sa fie goala\n");
		printf("  Primit: "); printList(list); printf("\n");
	}
	int firstNull = (list->first == NULL);
	int lastNull = (list->last == NULL);
	// Delete din lista goala (dupa stergere ultimul)
	deleteOnce(list, 'x');
	int afterNull = (list->first == NULL && list->last == NULL);
	free(list);
	sd_assert("deleteOnce - Test 5 picat (stergere nod unic, first)", firstNull);
	sd_assert("deleteOnce - Test 6 picat (stergere nod unic, last)", lastNull);
	sd_assert("deleteOnce - Test 7 picat (delete dupa stergere totala)", afterNull);

	// --- Construim [a, b, c, d, e] manual ---
	list = manualBuild("abcde");

	// --- Delete first ---
	deleteOnce(list, 'a');
	if (!manualCheck(list, "bcde")) {
		showMismatch(list, "bcde", "deleteOnce(['a'..'e'], 'a')");
		err = "deleteOnce - Test 8 picat (stergere first)"; goto cleanup_del1;
	}
	if (list->first->elem != 'b') {
		printf("\n  first->elem dupa stergere 'a': asteptat 'b', primit '%c'\n", list->first->elem);
		err = "deleteOnce - Test 9 picat (first != 'b')"; goto cleanup_del1;
	}
	if (list->first->prev != NULL) { err = "deleteOnce - Test 10 picat (first->prev != NULL)"; goto cleanup_del1; }
	if (!verifyLinks(list)) { err = "deleteOnce - Test 11 picat (links dupa stergere first)"; goto cleanup_del1; }

	// --- Delete last ---
	deleteOnce(list, 'e');
	if (!manualCheck(list, "bcd")) {
		showMismatch(list, "bcd", "deleteOnce(['b'..'e'], 'e')");
		err = "deleteOnce - Test 12 picat (stergere last)"; goto cleanup_del1;
	}
	if (list->last->elem != 'd') {
		printf("\n  last->elem dupa stergere 'e': asteptat 'd', primit '%c'\n", list->last->elem);
		err = "deleteOnce - Test 13 picat (last != 'd')"; goto cleanup_del1;
	}
	if (list->last->next != NULL) { err = "deleteOnce - Test 14 picat (last->next != NULL)"; goto cleanup_del1; }
	if (!verifyLinks(list)) { err = "deleteOnce - Test 15 picat (links dupa stergere last)"; goto cleanup_del1; }

	// --- Delete middle ---
	deleteOnce(list, 'c');
	if (!manualCheck(list, "bd")) {
		showMismatch(list, "bd", "deleteOnce(['b','c','d'], 'c')");
		err = "deleteOnce - Test 16 picat (stergere middle)"; goto cleanup_del1;
	}
	if (!verifyLinks(list)) { err = "deleteOnce - Test 17 picat (links dupa stergere middle)"; goto cleanup_del1; }

	// --- Delete pana la lista goala ---
	deleteOnce(list, 'b');
	if (!manualCheck(list, "d")) {
		showMismatch(list, "d", "deleteOnce(['b','d'], 'b')");
		err = "deleteOnce - Test 18 picat (dupa stergere -> [d])"; goto cleanup_del1;
	}
	if (list->first != list->last) {
		printf("\n  first != last pe lista cu 1 element\n");
		err = "deleteOnce - Test 19 picat (first != last pe 1 elem)"; goto cleanup_del1;
	}

	deleteOnce(list, 'd');
	if (list->first != NULL) { err = "deleteOnce - Test 20 picat (first != NULL dupa stergere tot)"; goto cleanup_del1; }
	if (list->last != NULL) { err = "deleteOnce - Test 21 picat (last != NULL dupa stergere tot)"; goto cleanup_del1; }

cleanup_del1:
	manualFree(list);
	if (err) return err;

	// --- Duplicate: sterge doar prima aparitie ---
	list = manualBuild("abaca");

	deleteOnce(list, 'a');
	if (!manualCheck(list, "baca")) {
		showMismatch(list, "baca", "deleteOnce(['a','b','a','c','a'], 'a')");
		err = "deleteOnce - Test 22 picat (prima aparitie duplicat)"; goto cleanup_del2;
	}
	if (list->first->elem != 'b') { err = "deleteOnce - Test 23 picat (first != 'b')"; goto cleanup_del2; }
	if (!verifyLinks(list)) { err = "deleteOnce - Test 24 picat (links duplicat 1)"; goto cleanup_del2; }

	deleteOnce(list, 'a');
	if (!manualCheck(list, "bca")) {
		showMismatch(list, "bca", "deleteOnce(['b','a','c','a'], 'a')");
		err = "deleteOnce - Test 25 picat (a doua aparitie duplicat)"; goto cleanup_del2;
	}
	if (!verifyLinks(list)) { err = "deleteOnce - Test 26 picat (links duplicat 2)"; goto cleanup_del2; }

	deleteOnce(list, 'a');
	if (!manualCheck(list, "bc")) {
		showMismatch(list, "bc", "deleteOnce(['b','c','a'], 'a')");
		err = "deleteOnce - Test 27 picat (ultima aparitie duplicat)"; goto cleanup_del2;
	}
	if (!verifyLinks(list)) { err = "deleteOnce - Test 28 picat (links duplicat 3)"; goto cleanup_del2; }

cleanup_del2:
	manualFree(list);
	if (err) return err;

	printf("deleteOnce \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: length
// =============================================================================
static char *test_length() {
	int res;

	// Lista goala
	List *list = createList();
	res = length(list);
	free(list);
	if (res != 0)
		printf("\n  length(lista goala): asteptat 0, primit %d\n", res);
	sd_assert("length - Test 1 picat (lista goala)", res == 0);

	// 1 element
	list = manualBuild("a");
	res = length(list);
	manualFree(list);
	if (res != 1)
		printf("\n  length(['a']): asteptat 1, primit %d\n", res);
	sd_assert("length - Test 2 picat (1 element)", res == 1);

	// 3 elemente
	list = manualBuild("abc");
	res = length(list);
	manualFree(list);
	if (res != 3)
		printf("\n  length(['a','b','c']): asteptat 3, primit %d\n", res);
	sd_assert("length - Test 3 picat (3 elemente)", res == 3);

	// 5 elemente
	list = manualBuild("abcde");
	res = length(list);
	manualFree(list);
	if (res != 5)
		printf("\n  length(['a'..'e']): asteptat 5, primit %d\n", res);
	sd_assert("length - Test 4 picat (5 elemente)", res == 5);

	// 10 elemente
	list = manualBuild("abcdefghij");
	res = length(list);
	manualFree(list);
	if (res != 10)
		printf("\n  length(10 elemente): asteptat 10, primit %d\n", res);
	sd_assert("length - Test 5 picat (10 elemente)", res == 10);

	// NULL
	res = length(NULL);
	if (res != 0)
		printf("\n  length(NULL): asteptat 0, primit %d\n", res);
	sd_assert("length - Test 6 picat (NULL)", res == 0);

	printf("length \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: destroyList
// =============================================================================
static char *test_destroyList() {
	// Lista goala
	List *list = createList();
	list = destroyList(list);
	if (list != NULL)
		printf("\n  destroyList(lista goala): asteptat NULL, primit non-NULL\n");
	sd_assert("destroyList - Test 1 picat (lista goala)", list == NULL);

	// 1 element
	list = manualBuild("x");
	list = destroyList(list);
	if (list != NULL)
		printf("\n  destroyList(1 elem): asteptat NULL, primit non-NULL\n");
	sd_assert("destroyList - Test 2 picat (1 element)", list == NULL);

	// 5 elemente
	list = manualBuild("abcde");
	list = destroyList(list);
	if (list != NULL)
		printf("\n  destroyList(5 elem): asteptat NULL, primit non-NULL\n");
	sd_assert("destroyList - Test 3 picat (5 elemente)", list == NULL);

	// 10 elemente
	list = manualBuild("abcdefghij");
	list = destroyList(list);
	if (list != NULL)
		printf("\n  destroyList(10 elem): asteptat NULL, primit non-NULL\n");
	sd_assert("destroyList - Test 4 picat (10 elemente)", list == NULL);

	printf("destroyList \t\t- Toate testele au trecut!\n");
	printf("  *Destroy se va verifica cu valgrind*");
	return 0;
}

// =============================================================================
// Test: isPalindrome
// =============================================================================
#include "palindrome.h"

static char *test_isPalindrome() {
	int res;
	List *list;
	char *err = NULL;

	// Lista goala
	list = createList();
	res = isPalindrome(list);
	free(list);
	if (res != 1)
		printf("\n  isPalindrome([]): asteptat 1 (palindrom), primit %d\n", res);
	sd_assert("isPalindrome - Test 1 picat (lista goala)", res == 1);

	// 1 caracter
	list = manualBuild("a");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"a\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 2 picat (\"a\")", res == 1);

	// 2 caractere - palindrom
	list = manualBuild("aa");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"aa\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 3 picat (\"aa\")", res == 1);

	// 2 caractere - nu e palindrom
	list = manualBuild("ab");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 0)
		printf("\n  isPalindrome(\"ab\"): asteptat 0, primit %d\n", res);
	sd_assert("isPalindrome - Test 4 picat (\"ab\")", res == 0);

	// Palindrom impar: "radar"
	list = manualBuild("radar");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"radar\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 5 picat (\"radar\")", res == 1);

	// Palindrom par: "noon"
	list = manualBuild("noon");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"noon\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 6 picat (\"noon\")", res == 1);

	// Nu e palindrom: "abcde"
	list = manualBuild("abcde");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 0)
		printf("\n  isPalindrome(\"abcde\"): asteptat 0, primit %d\n", res);
	sd_assert("isPalindrome - Test 7 picat (\"abcde\")", res == 0);

	// Nu e palindrom: "proffesor"
	list = manualBuild("proffesor");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 0)
		printf("\n  isPalindrome(\"proffesor\"): asteptat 0, primit %d\n", res);
	sd_assert("isPalindrome - Test 8 picat (\"proffesor\")", res == 0);

	// Palindrom: "level"
	list = manualBuild("level");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"level\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 9 picat (\"level\")", res == 1);

	// Nu e palindrom: "student"
	list = manualBuild("student");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 0)
		printf("\n  isPalindrome(\"student\"): asteptat 0, primit %d\n", res);
	sd_assert("isPalindrome - Test 10 picat (\"student\")", res == 0);

	// Palindrom: "racecar"
	list = manualBuild("racecar");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"racecar\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 11 picat (\"racecar\")", res == 1);

	// Nu e palindrom: "data-structures"
	list = manualBuild("data-structures");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 0)
		printf("\n  isPalindrome(\"data-structures\"): asteptat 0, primit %d\n", res);
	sd_assert("isPalindrome - Test 12 picat (\"data-structures\")", res == 0);

	// Palindrom lung: "abacaba"
	list = manualBuild("abacaba");
	res = isPalindrome(list);
	manualFree(list);
	if (res != 1)
		printf("\n  isPalindrome(\"abacaba\"): asteptat 1, primit %d\n", res);
	sd_assert("isPalindrome - Test 13 picat (\"abacaba\")", res == 1);

	printf("isPalindrome \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Main
// =============================================================================
static char *all_tests() {
	sd_run_test(test_create,       0.05);
	sd_run_test(test_isEmpty,      0.05);
	sd_run_test(test_contains,     0.10);
	sd_run_test(test_length,       0.10);
	sd_run_test(test_insertAt,     0.20);
	sd_run_test(test_deleteOnce,   0.20);
	sd_run_test(test_destroyList,  0.10);
	sd_run_test(test_isPalindrome, 0.20);
	return 0;
}

int main(void) {
	char *result = all_tests();

	printf("\n");
	if (result != 0) {
		printf("ESUAT: %s\n", result);
	}
	printf("Teste rulate: %d / %d\n", tests_run, 8);
	printf("SCOR: %.2f / 1.00\n\n", total_score);

	return result != 0;
}
