/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "tree.h"

float total_score = 0;
int all_passed = 1;
int has_graphviz = 0;

int test1[] = {9, 7, 5, 9, 4, 10, 6, 8, -8, 12};
int test2[] = {11, 9, 7, 25, 12, 29, 27, 8, 5, 1, 35, 26};
int test3[] = {26, 50, 150, 35, 175, 155, 100, 95, 9, 15, 45, 40, 4, 47, 98, \
	97, 99, 90, 200, 50, 68, 35, 39, 37, 44, 43, 46};

// ============================================================================
// Utilitare
// ============================================================================

// Verificare BST corecta: foloseste limite min/max (nu doar parinte-copil),
// verifica pointerii parent si detecteaza cicluri prin numararea nodurilor
int checkBSTHelper(Tree node, Tree expectedParent, long minVal, long maxVal,
	int *visited, int maxNodes) {
	if (node == NULL) return 1;
	if (*visited >= maxNodes) return 0; // prea multe noduri => ciclu
	(*visited)++;
	if (node->value <= minVal || node->value >= maxVal) return 0;
	if (node->parent != expectedParent) return 0;
	return checkBSTHelper(node->left, node, minVal, node->value, visited, maxNodes)
		&& checkBSTHelper(node->right, node, node->value, maxVal, visited, maxNodes);
}

int checkBST(Tree node, int expectedNodes) {
	int visited = 0;
	int valid = checkBSTHelper(node, NULL, (long)INT_MIN - 1, (long)INT_MAX + 1,
		&visited, expectedNodes + 1);
	return valid && (visited == expectedNodes);
}

void printArrayInline(int *v, int n) {
	int i;
	printf("[");
	for (i = 0; i < n; i++) {
		if (i > 0) printf(", ");
		printf("%d", v[i]);
	}
	printf("]");
}

int countNodesBounded(Tree root, int limit) {
	if (root == NULL || limit <= 0) return 0;
	return 1 + countNodesBounded(root->left, limit - 1)
		+ countNodesBounded(root->right, limit - 1);
}

Tree buildTree(int *testArr) {
	Tree tree = NULL;
	int i;
	for (i = 1; i <= testArr[0]; i++)
		tree = insert(tree, testArr[i]);
	return tree;
}

// ============================================================================
// GraphViz
// ============================================================================

void bst_print_dot_aux(Tree node, FILE *stream) {
	if (node->left) {
		fprintf(stream, "    %d -> %d;\n", node->value, node->left->value);
		bst_print_dot_aux(node->left, stream);
	}
	if (node->right) {
		fprintf(stream, "    %d -> %d;\n", node->value, node->right->value);
		bst_print_dot_aux(node->right, stream);
	}
}

void generatePNG(Tree tree, const char *filename, const char *color) {
	if (!has_graphviz || !tree) return;
	FILE *f = fopen("_tmp.dot", "w");
	fprintf(f, "digraph BST {\n");
	fprintf(f, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=%s];\n", color);
	if (!tree->right && !tree->left)
		fprintf(f, "    %d;\n", tree->value);
	else
		bst_print_dot_aux(tree, f);
	fprintf(f, "}\n");
	fclose(f);
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "dot _tmp.dot -Tpng -o %s 2>/dev/null", filename);
	system(cmd);
	remove("_tmp.dot");
}

void cleanPNGs() {
	system("rm -f tree*.png delete*.png lca*.png 2>/dev/null");
}

// ============================================================================
// Teste
// ============================================================================

int test_insert() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	int expectedNodes[] = {9, 11, 24};
	char *names[] = {"test1", "test2", "test3"};
	char pngName[32];
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		int ok = (tree != NULL && checkBST(tree, expectedNodes[t]));
		snprintf(pngName, sizeof(pngName), "tree%d.png", t + 1);
		generatePNG(tree, pngName, ok ? "green" : "orange");
		if (!ok) {
			printf("\n  insert %s PICAT\n", names[t]);
			printf("  Input (ordinea inserarii): ");
			printArrayInline(tests[t] + 1, tests[t][0]);
			printf("\n");
			printf("  Arborele rezultat nu respecta proprietatea BST,\n");
			printf("  pointerii parent sunt incorecti, sau numarul de noduri e gresit\n");
			passed = 0;
			if (tree != NULL) destroyTree(&tree);
			break;
		}
		destroyTree(&tree);
	}

	if (passed) {
		total_score += 1.0;
		if (has_graphviz)
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 1.00\t(vezi tree1/2/3.png)\n");
		else
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		if (has_graphviz)
			printf("insert \t\t- PICAT\t\t\t\t\t\t\t(vezi tree*.png)\n");
		else
			printf("insert \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_isEmpty_fn() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	for (t = 0; t < 3 && passed; t++) {
		tree = buildTree(tests[t]);
		if (isEmpty(tree) != 0) {
			printf("\n  isEmpty %s PICAT\n", names[t]);
			printf("  isEmpty(arbore nevid) -> asteptat: 0, primit: %d\n", isEmpty(tree));
			passed = 0;
		}
		destroyTree(&tree);
	}

	if (passed && isEmpty(NULL) != 1) {
		printf("\n  isEmpty test4 PICAT (arbore vid)\n");
		printf("  isEmpty(NULL) -> asteptat: 1, primit: %d\n", isEmpty(NULL));
		passed = 0;
	}

	if (passed) {
		total_score += 0.25;
		printf("isEmpty \t- Toate testele au trecut!\tPuncte: 0.25\n");
	} else {
		all_passed = 0;
		printf("isEmpty \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_contains_fn() {
	int passed = 1;
	Tree tree;
	int i;

	// Test1
	tree = buildTree(test1);
	int vals1_yes[] = {7, 10, 12, 6, -8, 5};
	int vals1_no[] = {2, 20, 11, 3};
	for (i = 0; i < 6 && passed; i++) {
		if (!contains(tree, vals1_yes[i])) {
			printf("\n  contains test1 PICAT\n");
			printf("  contains(tree, %d) -> asteptat: 1, primit: 0\n", vals1_yes[i]);
			passed = 0;
		}
	}
	for (i = 0; i < 4 && passed; i++) {
		if (contains(tree, vals1_no[i])) {
			printf("\n  contains test1 PICAT\n");
			printf("  contains(tree, %d) -> asteptat: 0, primit: 1\n", vals1_no[i]);
			passed = 0;
		}
	}
	destroyTree(&tree);

	// Test2
	if (passed) {
		tree = buildTree(test2);
		int vals2_yes[] = {25, 26, 1, 35};
		int vals2_no[] = {24, 28, 2, 36};
		for (i = 0; i < 4 && passed; i++) {
			if (!contains(tree, vals2_yes[i])) {
				printf("\n  contains test2 PICAT\n");
				printf("  contains(tree, %d) -> asteptat: 1, primit: 0\n", vals2_yes[i]);
				passed = 0;
			}
		}
		for (i = 0; i < 4 && passed; i++) {
			if (contains(tree, vals2_no[i])) {
				printf("\n  contains test2 PICAT\n");
				printf("  contains(tree, %d) -> asteptat: 0, primit: 1\n", vals2_no[i]);
				passed = 0;
			}
		}
		destroyTree(&tree);
	}

	// Test3
	if (passed) {
		tree = buildTree(test3);
		int vals3_yes[] = {155, 99, 43, 4, 200, 50};
		for (i = 0; i < 6 && passed; i++) {
			if (!contains(tree, vals3_yes[i])) {
				printf("\n  contains test3 PICAT\n");
				printf("  contains(tree, %d) -> asteptat: 1, primit: 0\n", vals3_yes[i]);
				passed = 0;
			}
		}
		destroyTree(&tree);
	}

	if (passed) {
		total_score += 0.25;
		printf("contains \t- Toate testele au trecut!\tPuncte: 0.25\n");
	} else {
		all_passed = 0;
		printf("contains \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_minmax() {
	int passed = 1;
	Tree tree, mn, mx;
	int *tests[] = {test1, test2, test3};
	int expectedMin[] = {-8, 1, 4};
	int expectedMax[] = {12, 35, 200};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	for (t = 0; t < 3 && passed; t++) {
		tree = buildTree(tests[t]);
		mn = minimum(tree);
		mx = maximum(tree);
		if (mn == NULL || mn->value != expectedMin[t]) {
			printf("\n  minimum %s PICAT\n", names[t]);
			if (mn)
				printf("  minimum() -> asteptat: %d, primit: %d\n",
					expectedMin[t], mn->value);
			else
				printf("  minimum() -> asteptat: %d, primit: NULL\n", expectedMin[t]);
			passed = 0;
		}
		if (passed && (mx == NULL || mx->value != expectedMax[t])) {
			printf("\n  maximum %s PICAT\n", names[t]);
			if (mx)
				printf("  maximum() -> asteptat: %d, primit: %d\n",
					expectedMax[t], mx->value);
			else
				printf("  maximum() -> asteptat: %d, primit: NULL\n", expectedMax[t]);
			passed = 0;
		}
		destroyTree(&tree);
	}

	// Test arbore vid
	if (passed) {
		mn = minimum(NULL);
		mx = maximum(NULL);
		if (mn != NULL || mx != NULL) {
			printf("\n  min/max test4 PICAT (arbore vid)\n");
			printf("  minimum(NULL) si maximum(NULL) trebuie sa returneze NULL\n");
			passed = 0;
		}
	}

	if (passed) {
		total_score += 1.0;
		printf("minmax \t\t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("minmax \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

typedef struct { int val; int expected; int has_result; } SuccPredTest;

int run_successor_tests(Tree tree, SuccPredTest *tests, int n, const char *tname) {
	Tree succ;
	int i;
	for (i = 0; i < n; i++) {
		succ = successor(tree, tests[i].val);
		if (tests[i].has_result) {
			if (succ == NULL || succ->value != tests[i].expected) {
				printf("\n  successor %s PICAT: successor(%d)\n", tname, tests[i].val);
				if (succ)
					printf("  Asteptat: %d, Primit: %d\n", tests[i].expected, succ->value);
				else
					printf("  Asteptat: %d, Primit: NULL\n", tests[i].expected);
				return 0;
			}
		} else if (succ != NULL) {
			printf("\n  successor %s PICAT: successor(%d)\n", tname, tests[i].val);
			printf("  Asteptat: NULL, Primit: %d\n", succ->value);
			return 0;
		}
	}
	return 1;
}

int test_successor_fn() {
	int passed = 1;
	Tree tree;

	// Test1
	tree = buildTree(test1);
	SuccPredTest st1[] = {{-8, 4, 1}, {6, 7, 1}, {7, 8, 1}, {12, 0, 0}};
	passed = run_successor_tests(tree, st1, 4, "test1");
	destroyTree(&tree);

	// Test2
	if (passed) {
		tree = buildTree(test2);
		SuccPredTest st2[] = {{1, 5, 1}, {8, 9, 1}, {12, 25, 1}, {9, 12, 1}, {35, 0, 0}};
		passed = run_successor_tests(tree, st2, 5, "test2");
		destroyTree(&tree);
	}

	// Test3
	if (passed) {
		tree = buildTree(test3);
		SuccPredTest st3[] = {{4, 9, 1}, {68, 90, 1}, {47, 50, 1}, {35, 37, 1},
			{200, 0, 0}, {99, 100, 1}};
		passed = run_successor_tests(tree, st3, 6, "test3");
		destroyTree(&tree);
	}

	if (passed) {
		total_score += 1.0;
		printf("successor \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("successor \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int run_predecessor_tests(Tree tree, SuccPredTest *tests, int n, const char *tname) {
	Tree pred;
	int i;
	for (i = 0; i < n; i++) {
		pred = predecessor(tree, tests[i].val);
		if (tests[i].has_result) {
			if (pred == NULL || pred->value != tests[i].expected) {
				printf("\n  predecessor %s PICAT: predecessor(%d)\n", tname, tests[i].val);
				if (pred)
					printf("  Asteptat: %d, Primit: %d\n", tests[i].expected, pred->value);
				else
					printf("  Asteptat: %d, Primit: NULL\n", tests[i].expected);
				return 0;
			}
		} else if (pred != NULL) {
			printf("\n  predecessor %s PICAT: predecessor(%d)\n", tname, tests[i].val);
			printf("  Asteptat: NULL, Primit: %d\n", pred->value);
			return 0;
		}
	}
	return 1;
}

int test_predecessor_fn() {
	int passed = 1;
	Tree tree;

	// Test1
	tree = buildTree(test1);
	SuccPredTest pt1[] = {{4, -8, 1}, {7, 6, 1}, {8, 7, 1}, {-8, 0, 0}};
	passed = run_predecessor_tests(tree, pt1, 4, "test1");
	destroyTree(&tree);

	// Test2
	if (passed) {
		tree = buildTree(test2);
		SuccPredTest pt2[] = {{5, 1, 1}, {9, 8, 1}, {25, 12, 1}, {12, 9, 1}, {26, 25, 1}};
		passed = run_predecessor_tests(tree, pt2, 5, "test2");
		destroyTree(&tree);
	}

	// Test3
	if (passed) {
		tree = buildTree(test3);
		SuccPredTest pt3[] = {{9, 4, 1}, {90, 68, 1}, {50, 47, 1}, {37, 35, 1},
			{4, 0, 0}, {100, 99, 1}, {68, 50, 1}};
		passed = run_predecessor_tests(tree, pt3, 7, "test3");
		destroyTree(&tree);
	}

	if (passed) {
		total_score += 1.0;
		printf("predecessor \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("predecessor \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_delete_fn() {
	int passed = 1;
	Tree tree;

	// Test1: stergere frunza (12 din test1)
	tree = buildTree(test1);
	tree = delete(tree, 12);
	if (tree == NULL || !checkBST(tree, 8) || contains(tree, 12)) {
		printf("\n  delete test1 PICAT (stergere frunza: 12)\n");
		printf("  Arborele dupa stergere nu respecta BST, pointerii parent sunt\n");
		printf("  incorecti, numarul de noduri e gresit, sau valoarea inca exista\n");
		passed = 0;
	}
	generatePNG(tree, "delete1.png", passed ? "green" : "orange");
	if (tree) destroyTree(&tree);

	// Test2: stergere nod cu doi copii (29 din test2)
	if (passed) {
		tree = buildTree(test2);
		tree = delete(tree, 29);
		if (tree == NULL || !checkBST(tree, 10) || contains(tree, 29)) {
			printf("\n  delete test2 PICAT (stergere nod cu doi copii: 29)\n");
			printf("  Arborele dupa stergere nu respecta BST, pointerii parent sunt\n");
			printf("  incorecti, numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "delete2.png", passed ? "green" : "orange");
		if (tree) destroyTree(&tree);
	}

	// Test3: stergere radacina (50 din test3)
	if (passed) {
		tree = buildTree(test3);
		tree = delete(tree, 50);
		if (tree == NULL || !checkBST(tree, 23) || contains(tree, 50)) {
			printf("\n  delete test3 PICAT (stergere radacina: 50)\n");
			printf("  Arborele dupa stergere nu respecta BST, pointerii parent sunt\n");
			printf("  incorecti, numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "delete3.png", passed ? "green" : "orange");
		if (tree) destroyTree(&tree);
	}

	if (passed) {
		total_score += 2.0;
		printf("delete \t\t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		printf("delete \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_lca_fn() {
	int passed = 1;
	Tree tree, lca;

	// Test1: lca(12, -8) in test1 => 7
	tree = buildTree(test1);
	lca = lowestCommonAncestor(tree, 12, -8);
	if (lca == NULL || lca->value != 7) {
		printf("\n  lca test1 PICAT: lca(12, -8)\n");
		if (lca)
			printf("  Asteptat: 7, Primit: %d\n", lca->value);
		else
			printf("  Asteptat: 7, Primit: NULL\n");
		passed = 0;
	}
	destroyTree(&tree);

	// Test2: lca(12, 35) in test2 => 25
	if (passed) {
		tree = buildTree(test2);
		lca = lowestCommonAncestor(tree, 12, 35);
		if (lca == NULL || lca->value != 25) {
			printf("\n  lca test2 PICAT: lca(12, 35)\n");
			if (lca)
				printf("  Asteptat: 25, Primit: %d\n", lca->value);
			else
				printf("  Asteptat: 25, Primit: NULL\n");
			passed = 0;
		}
		destroyTree(&tree);
	}

	// Test3: lca(4, 43) in test3 => 35
	if (passed) {
		tree = buildTree(test3);
		lca = lowestCommonAncestor(tree, 4, 43);
		if (lca == NULL || lca->value != 35) {
			printf("\n  lca test3 PICAT: lca(4, 43)\n");
			if (lca)
				printf("  Asteptat: 35, Primit: %d\n", lca->value);
			else
				printf("  Asteptat: 35, Primit: NULL\n");
			passed = 0;
		}
		destroyTree(&tree);
	}

	if (passed) {
		total_score += 2.0;
		printf("lca \t\t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		printf("lca \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_destroyTree_fn() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	// Test 1-3: destroyTree pe arbori nevizi, root trebuie sa devina NULL
	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		destroyTree(&tree);
		if (tree != NULL) {
			printf("\n  destroyTree %s PICAT\n", names[t]);
			printf("  Dupa destroyTree, root trebuie sa fie NULL, dar nu este\n");
			passed = 0;
		}
	}

	// Test 4: destroyTree pe arbore vid (NULL) - nu trebuie sa faca crash
	if (passed) {
		tree = NULL;
		destroyTree(&tree);
		if (tree != NULL) {
			printf("\n  destroyTree test4 PICAT (arbore vid)\n");
			printf("  destroyTree(&NULL) nu trebuie sa modifice pointerul\n");
			passed = 0;
		}
	}

	// Test 5: apel dublu - dupa primul destroyTree, al doilea pe NULL
	// nu trebuie sa faca crash
	if (passed) {
		tree = buildTree(test1);
		destroyTree(&tree);
		destroyTree(&tree); // al doilea apel pe NULL
		if (tree != NULL) {
			printf("\n  destroyTree test5 PICAT (apel dublu)\n");
			printf("  Al doilea destroyTree nu trebuie sa faca crash\n");
			passed = 0;
		}
	}

	// Test 6: destroyTree pe arbore cu un singur nod
	if (passed) {
		tree = insert(NULL, 42);
		destroyTree(&tree);
		if (tree != NULL) {
			printf("\n  destroyTree test6 PICAT (un singur nod)\n");
			printf("  Dupa destroyTree, root trebuie sa fie NULL, dar nu este\n");
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.75;
		printf("destroyTree \t- Toate testele au trecut!\tPuncte: 0.75\n");
	} else {
		all_passed = 0;
		printf("destroyTree \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

void run_all() {
	test_insert();
	test_isEmpty_fn();
	test_contains_fn();
	test_minmax();
	test_successor_fn();
	test_predecessor_fn();
	test_delete_fn();
	test_lca_fn();
	test_destroyTree_fn();
}

void run_selective(int argc, char **argv) {
	int i;
	int viz[10] = {0};
	for (i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		if (n < 1 || n > 9 || viz[n]) continue;
		viz[n] = 1;
		switch (n) {
			case 1: test_insert(); break;
			case 2: test_isEmpty_fn(); break;
			case 3: test_contains_fn(); break;
			case 4: test_minmax(); break;
			case 5: test_successor_fn(); break;
			case 6: test_predecessor_fn(); break;
			case 7: test_delete_fn(); break;
			case 8: test_lca_fn(); break;
			case 9: test_destroyTree_fn(); break;
		}
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));
	has_graphviz = (system("which dot > /dev/null 2>&1") == 0);
	if (!has_graphviz) {
		printf("(!) graphviz nu este instalat - nu se vor genera imagini cu arborii\n");
		printf("    Instalare: sudo apt install graphviz (Linux) / brew install graphviz (macOS)\n\n");
	}
	cleanPNGs();

	if (argc == 1)
		run_all();
	else
		run_selective(argc, argv);

	printf("SCORE:%.2f\n", total_score);
	return all_passed ? 0 : 1;
}
