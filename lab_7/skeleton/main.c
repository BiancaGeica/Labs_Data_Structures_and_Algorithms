/*
*	Created by Nan Mihai on 11.04.2023
*	Copyright (c) 2023 Nan Mihai. All rights reserved.
*	Laborator 7 - Structuri de date și algoritmi
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2022-2023, Seria CD
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "avl.h"

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
	printf("  !! Programul s-a oprit in timpul testului");
	if (current_test[0] != '\0')
		printf(": %s", current_test);
	printf(" !!\n");
	printf("  !! Verificati accesul la memorie in implementare !!\n\n");
	printf("SCORE:%.2f\n", total_score);
	_exit(128 + sig);
}

int test1[] = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int test2[] = {22, 10, 12, 5, 90, 80, 70, 60, 50, 40, 41, 42, 43, 44, 45, 46, 47, 38, 37, 36, 35, 34, 33};
int test3[] = {10, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};

// ============================================================================
// Utilitare
// ============================================================================

int my_abs(int x) { return x >= 0 ? x : -x; }

int rec_height(Tree root) {
	if (root == NULL) return -1;
	int l = rec_height(root->left);
	int r = rec_height(root->right);
	return 1 + (l > r ? l : r);
}

// Verificare AVL completa: BST, heights corecte, echilibrat, detectie cicluri
int checkAVLHelper(Tree node, long minVal, long maxVal,
	int *visited, int maxNodes) {
	if (node == NULL) return 1;
	if (*visited >= maxNodes) return 0;
	(*visited)++;
	if (node->value <= minVal || node->value >= maxVal) return 0;
	if (node->height != rec_height(node)) return 0;
	int lh = node->left ? node->left->height : -1;
	int rh = node->right ? node->right->height : -1;
	if (my_abs(rh - lh) > 1) return 0;
	return checkAVLHelper(node->left, minVal, node->value, visited, maxNodes)
		&& checkAVLHelper(node->right, node->value, maxVal, visited, maxNodes);
}

int checkAVL(Tree root, int expectedNodes) {
	int visited = 0;
	int valid = checkAVLHelper(root, (long)INT_MIN - 1, (long)INT_MAX + 1,
		&visited, expectedNodes + 1);
	return valid && (visited == expectedNodes);
}

int checker_contains(Tree root, int value) {
	if (!root) return 0;
	if (root->value == value) return 1;
	if (value < root->value) return checker_contains(root->left, value);
	return checker_contains(root->right, value);
}

// Cleanup intern (nu depinde de freeTree-ul studentului)
void checker_free(Tree root) {
	if (root == NULL) return;
	checker_free(root->left);
	checker_free(root->right);
	free(root);
}

Tree buildTree(int *testArr) {
	Tree tree = NULL;
	int i;
	for (i = 1; i <= testArr[0]; i++)
		tree = insert(tree, testArr[i]);
	return tree;
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

// ============================================================================
// GraphViz
// ============================================================================

void avl_print_dot_aux(Tree node, FILE *stream) {
	if (node->left) {
		fprintf(stream, "    %d -> %d;\n", node->value, node->left->value);
		avl_print_dot_aux(node->left, stream);
	}
	if (node->right) {
		fprintf(stream, "    %d -> %d;\n", node->value, node->right->value);
		avl_print_dot_aux(node->right, stream);
	}
}

void generatePNG(Tree tree, const char *filename, const char *color) {
	if (!has_graphviz || !tree) return;
	FILE *f = fopen("_tmp.dot", "w");
	fprintf(f, "digraph AVL {\n");
	fprintf(f, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=%s];\n", color);
	if (!tree->right && !tree->left)
		fprintf(f, "    %d;\n", tree->value);
	else
		avl_print_dot_aux(tree, f);
	fprintf(f, "}\n");
	fclose(f);
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "dot _tmp.dot -Tpng -o %s 2>/dev/null", filename);
	system(cmd);
	remove("_tmp.dot");
}

void cleanPNGs() {
	system("rm -f _tmp.dot 2>/dev/null");
}

// ============================================================================
// Teste
// ============================================================================

int test_helpers() {
	int passed = 1;

	// Test createTree
	Tree node = createTree(42);
	if (node == NULL) {
		printf("\n  helpers PICAT: createTree(42) a returnat NULL\n");
		passed = 0;
	}

	if (passed && node->value != 42) {
		printf("\n  helpers PICAT: createTree(42)->value = %d, asteptat 42\n",
			node->value);
		passed = 0;
	}

	if (passed && node->height != 0) {
		printf("\n  helpers PICAT: createTree(42)->height = %d, asteptat 0\n",
			node->height);
		passed = 0;
	}

	if (passed && (node->left != NULL || node->right != NULL)) {
		printf("\n  helpers PICAT: createTree(42) left/right nu sunt NULL\n");
		passed = 0;
	}

	// Test height
	if (passed && height(NULL) != -1) {
		printf("\n  helpers PICAT: height(NULL) = %d, asteptat -1\n",
			height(NULL));
		passed = 0;
	}

	if (passed && height(node) != 0) {
		printf("\n  helpers PICAT: height(frunza) = %d, asteptat 0\n",
			height(node));
		passed = 0;
	}

	// Test balancedFactor
	if (passed && balancedFactor(NULL) != 0) {
		printf("\n  helpers PICAT: balancedFactor(NULL) = %d, asteptat 0\n",
			balancedFactor(NULL));
		passed = 0;
	}

	if (passed) {
		// Construiesc arbore manual: node(42, left=20, right=NULL)
		node->left = createTree(20);
		if (node->left == NULL) {
			printf("\n  helpers PICAT: createTree(20) a returnat NULL\n");
			passed = 0;
		}
	}

	if (passed) {
		updateHeight(node);
		// height(node) ar trebui sa fie 1
		if (node->height != 1) {
			printf("\n  helpers PICAT: updateHeight nu actualizeaza corect\n");
			printf("  Nod cu un copil stang (frunza) -> asteptat height=1, primit=%d\n",
				node->height);
			passed = 0;
		}
	}

	if (passed) {
		// BF(node) = height(right) - height(left) = -1 - 0 = -1
		int bf = balancedFactor(node);
		if (bf != -1) {
			printf("\n  helpers PICAT: balancedFactor(nod cu un copil stang)\n");
			printf("  Asteptat: -1, Primit: %d\n", bf);
			passed = 0;
		}
	}

	if (node) checker_free(node);

	if (passed) {
		total_score += 0.50;
		printf("helpers \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("helpers \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_minimum_fn() {
	int passed = 1;

	// Test NULL
	Tree mn = minimum(NULL);
	if (mn != NULL) {
		printf("\n  minimum PICAT: minimum(NULL) != NULL\n");
		passed = 0;
	}

	// Test frunza
	if (passed) {
		Tree leaf = createTree(10);
		mn = minimum(leaf);
		if (mn == NULL || mn->value != 10) {
			printf("\n  minimum PICAT: minimum(frunza cu valoarea 10)\n");
			if (mn)
				printf("  Asteptat: 10, Primit: %d\n", mn->value);
			else
				printf("  Asteptat: 10, Primit: NULL\n");
			passed = 0;
		}
		checker_free(leaf);
	}

	// Test arbore manual: 10(left=5(left=3(left=1), right=7), right=15)
	if (passed) {
		Tree root = createTree(10);
		root->left = createTree(5);
		root->right = createTree(15);
		root->left->left = createTree(3);
		root->left->right = createTree(7);
		root->left->left->left = createTree(1);

		mn = minimum(root);
		if (mn == NULL || mn->value != 1) {
			printf("\n  minimum PICAT: minimum(arbore) -> asteptat 1\n");
			if (mn)
				printf("  Primit: %d\n", mn->value);
			else
				printf("  Primit: NULL\n");
			passed = 0;
		}
		checker_free(root);
	}

	if (passed) {
		total_score += 0.50;
		printf("minimum \t- Toate testele au trecut!\tPuncte: 0.50\n");
	} else {
		all_passed = 0;
		printf("minimum \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_rightRotation_fn() {
	int passed = 1;

	// Test 1: rotatie simpla  8(left=5(left=4)) => 5(left=4, right=8)
	Tree root = createTree(8);
	if (root == NULL) {
		printf("\n  rightRotation PICAT: createTree a returnat NULL\n");
		passed = 0;
	}

	if (passed) {
		root->left = createTree(5);
		root->left->left = createTree(4);
		root->left->height = 1;
		root->height = 2;

		root = rightRotation(root);

		if (root == NULL || root->value != 5) {
			printf("\n  rightRotation test1 PICAT: radacina dupa rotatie\n");
			printf("  Asteptat: 5, Primit: %s\n",
				root ? "valoare gresita" : "NULL");
			passed = 0;
		}

		if (passed && (root->left == NULL || root->left->value != 4)) {
			printf("\n  rightRotation test1 PICAT: copil stang\n");
			printf("  Asteptat: 4\n");
			passed = 0;
		}

		if (passed && (root->right == NULL || root->right->value != 8)) {
			printf("\n  rightRotation test1 PICAT: copil drept\n");
			printf("  Asteptat: 8\n");
			passed = 0;
		}

		// Verifica heights
		if (passed && root->right->height != 0) {
			printf("\n  rightRotation test1 PICAT: height(8) = %d, asteptat 0\n",
				root->right->height);
			passed = 0;
		}

		if (passed && root->height != 1) {
			printf("\n  rightRotation test1 PICAT: height(5) = %d, asteptat 1\n",
				root->height);
			passed = 0;
		}

		if (root) checker_free(root);
	}

	// Test 2: rotatie cu subtree B transferat
	// 10(left=5(left=3(left=1),right=7),right=15) => 5(left=3(left=1),right=10(left=7,right=15))
	if (passed) {
		root = createTree(10);
		root->right = createTree(15);
		root->left = createTree(5);
		root->left->left = createTree(3);
		root->left->right = createTree(7);
		root->left->left->left = createTree(1);
		root->left->left->left->height = 0;
		root->left->left->height = 1;
		root->left->right->height = 0;
		root->left->height = 2;
		root->right->height = 0;
		root->height = 3;

		root = rightRotation(root);

		if (root == NULL || root->value != 5) {
			printf("\n  rightRotation test2 PICAT: radacina dupa rotatie\n");
			printf("  Asteptat: 5\n");
			passed = 0;
		}

		// Verificam ca subarborele B (7) a fost mutat corect
		if (passed && (root->right == NULL || root->right->value != 10)) {
			printf("\n  rightRotation test2 PICAT: copil drept\n");
			printf("  Asteptat: 10\n");
			passed = 0;
		}

		if (passed && (root->right->left == NULL || root->right->left->value != 7)) {
			printf("\n  rightRotation test2 PICAT: subarborele B nu a fost transferat\n");
			printf("  10->left trebuie sa fie 7\n");
			passed = 0;
		}

		if (passed && (root->right->right == NULL || root->right->right->value != 15)) {
			printf("\n  rightRotation test2 PICAT: subarborele C pierdut\n");
			printf("  10->right trebuie sa fie 15\n");
			passed = 0;
		}

		// Verificam heights dupa rotatie
		if (passed && root->right->height != 1) {
			printf("\n  rightRotation test2 PICAT: height(10) = %d, asteptat 1\n",
				root->right->height);
			passed = 0;
		}

		if (passed && root->height != 2) {
			printf("\n  rightRotation test2 PICAT: height(5) = %d, asteptat 2\n",
				root->height);
			passed = 0;
		}

		if (root) checker_free(root);
	}

	if (passed) {
		total_score += 1.0;
		printf("rightRotation \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("rightRotation \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_leftRotation_fn() {
	int passed = 1;

	// Test 1: rotatie simpla  4(right=6(right=8)) => 6(left=4, right=8)
	Tree root = createTree(4);
	if (root == NULL) {
		printf("\n  leftRotation PICAT: createTree a returnat NULL\n");
		passed = 0;
	}

	if (passed) {
		root->right = createTree(6);
		root->right->right = createTree(8);
		root->right->height = 1;
		root->height = 2;

		root = leftRotation(root);

		if (root == NULL || root->value != 6) {
			printf("\n  leftRotation test1 PICAT: radacina dupa rotatie\n");
			printf("  Asteptat: 6, Primit: %s\n",
				root ? "valoare gresita" : "NULL");
			passed = 0;
		}

		if (passed && (root->left == NULL || root->left->value != 4)) {
			printf("\n  leftRotation test1 PICAT: copil stang\n");
			printf("  Asteptat: 4\n");
			passed = 0;
		}

		if (passed && (root->right == NULL || root->right->value != 8)) {
			printf("\n  leftRotation test1 PICAT: copil drept\n");
			printf("  Asteptat: 8\n");
			passed = 0;
		}

		if (passed && root->left->height != 0) {
			printf("\n  leftRotation test1 PICAT: height(4) = %d, asteptat 0\n",
				root->left->height);
			passed = 0;
		}

		if (passed && root->height != 1) {
			printf("\n  leftRotation test1 PICAT: height(6) = %d, asteptat 1\n",
				root->height);
			passed = 0;
		}

		if (root) checker_free(root);
	}

	// Test 2: rotatie cu subtree B transferat
	// 5(left=3,right=10(left=7,right=15(right=20))) => 10(left=5(left=3,right=7),right=15(right=20))
	if (passed) {
		root = createTree(5);
		root->left = createTree(3);
		root->right = createTree(10);
		root->right->left = createTree(7);
		root->right->right = createTree(15);
		root->right->right->right = createTree(20);
		// Set correct heights
		root->left->height = 0;               // 3
		root->right->left->height = 0;        // 7
		root->right->right->right->height = 0; // 20
		root->right->right->height = 1;       // 15
		root->right->height = 2;              // 10
		root->height = 3;                     // 5

		root = leftRotation(root);

		if (root == NULL || root->value != 10) {
			printf("\n  leftRotation test2 PICAT: radacina dupa rotatie\n");
			printf("  Asteptat: 10\n");
			passed = 0;
		}

		if (passed && (root->left == NULL || root->left->value != 5)) {
			printf("\n  leftRotation test2 PICAT: copil stang\n");
			printf("  Asteptat: 5\n");
			passed = 0;
		}

		if (passed && (root->left->right == NULL || root->left->right->value != 7)) {
			printf("\n  leftRotation test2 PICAT: subarborele B nu a fost transferat\n");
			printf("  5->right trebuie sa fie 7\n");
			passed = 0;
		}

		if (passed && (root->left->left == NULL || root->left->left->value != 3)) {
			printf("\n  leftRotation test2 PICAT: subarborele A pierdut\n");
			printf("  5->left trebuie sa fie 3\n");
			passed = 0;
		}

		if (passed && root->left->height != 1) {
			printf("\n  leftRotation test2 PICAT: height(5) = %d, asteptat 1\n",
				root->left->height);
			passed = 0;
		}

		if (passed && root->height != 2) {
			printf("\n  leftRotation test2 PICAT: height(10) = %d, asteptat 2\n",
				root->height);
			passed = 0;
		}

		if (root) checker_free(root);
	}

	if (passed) {
		total_score += 1.0;
		printf("leftRotation \t- Toate testele au trecut!\tPuncte: 1.00\n");
	} else {
		all_passed = 0;
		printf("leftRotation \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_insert_fn() {
	int passed = 1;
	Tree tree;
	int t, i;

	// Test1: cazul RR (inserare 1,2,3 - toate la dreapta)
	snprintf(current_test, sizeof(current_test), "insert test1 (RR: 1,2,3)");
	printf("RUNNING:%s\n", current_test);
	tree = NULL;
	tree = insert(tree, 1);
	tree = insert(tree, 2);
	tree = insert(tree, 3);
	if (tree == NULL || !checkAVL(tree, 3) || tree->value != 2) {
		printf("\n  insert test1 PICAT (cazul RR: inserare 1,2,3)\n");
		printf("  Asteptat radacina: 2\n");
		generatePNG(tree, "avl_insert_RR.png", "orange");
		passed = 0;
	}
	if (tree) checker_free(tree);

	// Test2: cazul LL (inserare 3,2,1 - toate la stanga)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "insert test2 (LL: 3,2,1)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 3);
		tree = insert(tree, 2);
		tree = insert(tree, 1);
		if (tree == NULL || !checkAVL(tree, 3) || tree->value != 2) {
			printf("\n  insert test2 PICAT (cazul LL: inserare 3,2,1)\n");
			printf("  Asteptat radacina: 2\n");
			generatePNG(tree, "avl_insert_LL.png", "orange");
			passed = 0;
		}
		if (tree) checker_free(tree);
	}

	// Test3: cazul LR (inserare 3,1,2)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "insert test3 (LR: 3,1,2)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 3);
		tree = insert(tree, 1);
		tree = insert(tree, 2);
		if (tree == NULL || !checkAVL(tree, 3) || tree->value != 2) {
			printf("\n  insert test3 PICAT (cazul LR: inserare 3,1,2)\n");
			printf("  Asteptat radacina: 2\n");
			generatePNG(tree, "avl_insert_LR.png", "orange");
			passed = 0;
		}
		if (tree) checker_free(tree);
	}

	// Test4: cazul RL (inserare 1,3,2)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "insert test4 (RL: 1,3,2)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 1);
		tree = insert(tree, 3);
		tree = insert(tree, 2);
		if (tree == NULL || !checkAVL(tree, 3) || tree->value != 2) {
			printf("\n  insert test4 PICAT (cazul RL: inserare 1,3,2)\n");
			printf("  Asteptat radacina: 2\n");
			generatePNG(tree, "avl_insert_RL.png", "orange");
			passed = 0;
		}
		if (tree) checker_free(tree);
	}

	// Test5: inserare duplicat (nu trebuie adaugat)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "insert test5 (duplicat)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 5);
		tree = insert(tree, 3);
		tree = insert(tree, 7);
		tree = insert(tree, 5);
		if (tree == NULL || !checkAVL(tree, 3)) {
			printf("\n  insert test5 PICAT (inserare duplicat)\n");
			printf("  Arborele nu trebuie sa contina duplicate\n");
			generatePNG(tree, "avl_insert_dup.png", "orange");
			passed = 0;
		}
		if (tree) checker_free(tree);
	}

	// Test6-8: inserari mari (test1, test2, test3)
	int *tests[] = {test1, test2, test3};
	int expectedNodes[] = {10, 22, 10};
	char *names[] = {"test6", "test7", "test8"};
	char pngName[32];

	for (t = 0; t < 3 && passed; t++) {
		snprintf(current_test, sizeof(current_test), "insert %s", names[t]);
	printf("RUNNING:%s\n", current_test);
		tree = buildTree(tests[t]);

		int found = 1;
		for (i = 1; i <= tests[t][0]; i++) {
			if (!checker_contains(tree, tests[t][i])) {
				found = 0;
				break;
			}
		}

		int ok = (tree != NULL && found && checkAVL(tree, expectedNodes[t]));

		snprintf(pngName, sizeof(pngName), "avl_insert_%d.png", t + 1);
		generatePNG(tree, pngName, ok ? "green" : "orange");

		if (!ok) {
			printf("\n  insert %s PICAT\n", names[t]);
			if (tree == NULL) {
				printf("  Arborele returnat este NULL\n");
			} else if (!found) {
				printf("  Valoarea %d nu a fost gasita in arbore dupa inserare\n",
					tests[t][i]);
			} else {
				printf("  Arborele nu respecta proprietatile AVL (BST/echilibru/heights)\n");
			}
			passed = 0;
			if (tree) checker_free(tree);
			break;
		}
		checker_free(tree);
	}

	if (passed) {
		total_score += 2.0;
		if (has_graphviz)
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 2.00\t(vezi avl_insert_*.png)\n");
		else
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		if (has_graphviz)
			printf("insert \t\t- PICAT\t\t\t\t\t\t\t(vezi avl_insert_*.png)\n");
		else
			printf("insert \t\t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_delete_fn() {
	int passed = 1;
	Tree tree, orig;

	// Test1: stergere frunza (10 din test1)
	snprintf(current_test, sizeof(current_test), "delete test1 (frunza: 10 din test1)");
	printf("RUNNING:%s\n", current_test);
	tree = buildTree(test1);
	orig = tree;
	tree = delete(tree, 10);
	if (tree == NULL || !checkAVL(tree, 9) || checker_contains(tree, 10)) {
		printf("\n  delete test1 PICAT (stergere frunza: 10)\n");
		printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
		printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
		passed = 0;
	}
	generatePNG(tree, "avl_delete1.png", passed ? "green" : "orange");
	if (tree) checker_free(tree);
	else if (orig) checker_free(orig);

	// Test2: stergere nod intern (42 din test2)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test2 (nod intern: 42 din test2)");
	printf("RUNNING:%s\n", current_test);
		tree = buildTree(test2);
		orig = tree;
		tree = delete(tree, 42);
		if (tree == NULL || !checkAVL(tree, 21) || checker_contains(tree, 42)) {
			printf("\n  delete test2 PICAT (stergere nod intern: 42)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "avl_delete2.png", passed ? "green" : "orange");
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test3: stergere din test3 (50)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test3 (stergere 50 din test3)");
	printf("RUNNING:%s\n", current_test);
		tree = buildTree(test3);
		orig = tree;
		tree = delete(tree, 50);
		if (tree == NULL || !checkAVL(tree, 9) || checker_contains(tree, 50)) {
			printf("\n  delete test3 PICAT (stergere: 50)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "avl_delete3.png", passed ? "green" : "orange");
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test4: cazul LR
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test4 (cazul LR: insert 10,5,15,7 -> delete 15)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 5);
		tree = insert(tree, 15);
		tree = insert(tree, 7);
		orig = tree;
		tree = delete(tree, 15);
		if (tree == NULL || !checkAVL(tree, 3) || checker_contains(tree, 15)) {
			printf("\n  delete test4 PICAT (cazul LR: stergere 15)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "avl_delete4.png", passed ? "green" : "orange");
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test5: cazul RL
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test5 (cazul RL: insert 10,15,5,12 -> delete 5)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 15);
		tree = insert(tree, 5);
		tree = insert(tree, 12);
		orig = tree;
		tree = delete(tree, 5);
		if (tree == NULL || !checkAVL(tree, 3) || checker_contains(tree, 5)) {
			printf("\n  delete test5 PICAT (cazul RL: stergere 5)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		generatePNG(tree, "avl_delete5.png", passed ? "green" : "orange");
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test6: cazul LL
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test6 (cazul LL: insert 10,5,15,2 -> delete 15)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 5);
		tree = insert(tree, 15);
		tree = insert(tree, 2);
		orig = tree;
		tree = delete(tree, 15);
		if (tree == NULL || !checkAVL(tree, 3) || checker_contains(tree, 15)) {
			printf("\n  delete test6 PICAT (cazul LL: stergere 15)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test7: cazul RR
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test7 (cazul RR: insert 10,15,5,20 -> delete 5)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 15);
		tree = insert(tree, 5);
		tree = insert(tree, 20);
		orig = tree;
		tree = delete(tree, 5);
		if (tree == NULL || !checkAVL(tree, 3) || checker_contains(tree, 5)) {
			printf("\n  delete test7 PICAT (cazul RR: stergere 5)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test8: stergere radacina (nod cu doi copii)
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test8 (stergere radacina: 10)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 5);
		tree = insert(tree, 15);
		tree = insert(tree, 3);
		tree = insert(tree, 7);
		tree = insert(tree, 12);
		tree = insert(tree, 20);
		orig = tree;
		tree = delete(tree, 10);
		if (tree == NULL || !checkAVL(tree, 6) || checker_contains(tree, 10)) {
			printf("\n  delete test8 PICAT (stergere radacina)\n");
			printf("  Arborele dupa stergere nu respecta proprietatile AVL,\n");
			printf("  numarul de noduri e gresit, sau valoarea inca exista\n");
			passed = 0;
		}
		if (tree) checker_free(tree);
		else if (orig) checker_free(orig);
	}

	// Test9: stergere din arbore cu un singur nod
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test9 (stergere nod unic: 42)");
	printf("RUNNING:%s\n", current_test);
		tree = insert(NULL, 42);
		orig = tree;
		tree = delete(tree, 42);
		if (tree != NULL) {
			printf("\n  delete test9 PICAT (stergere nod unic)\n");
			printf("  Arborele trebuie sa fie NULL dupa stergerea unicului nod\n");
			passed = 0;
			checker_free(tree);
		}
	}

	// Test10: stergere valoare inexistenta
	if (passed) {
		snprintf(current_test, sizeof(current_test), "delete test10 (stergere valoare inexistenta: 99)");
	printf("RUNNING:%s\n", current_test);
		tree = NULL;
		tree = insert(tree, 10);
		tree = insert(tree, 5);
		tree = insert(tree, 15);
		tree = delete(tree, 99);
		if (tree == NULL || !checkAVL(tree, 3)) {
			printf("\n  delete test10 PICAT (stergere valoare inexistenta)\n");
			printf("  Arborele nu trebuie modificat la stergerea unei valori inexistente\n");
			passed = 0;
		}
		if (tree) checker_free(tree);
	}

	if (passed) {
		total_score += 4.0;
		if (has_graphviz)
			printf("delete \t\t- Toate testele au trecut!\tPuncte: 4.00\t(vezi avl_delete_*.png)\n");
		else
			printf("delete \t\t- Toate testele au trecut!\tPuncte: 4.00\n");
	} else {
		all_passed = 0;
		if (has_graphviz)
			printf("delete \t\t- PICAT\t\t\t\t\t\t\t(vezi avl_delete_*.png)\n");
		else
			printf("delete \t\t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

int test_freeTree_fn() {
	int passed = 1;
	Tree tree;

	// Test 1: freeTree pe arbore manual (7 noduri)
	tree = createTree(10);
	if (tree) {
		tree->left = createTree(5);
		tree->right = createTree(15);
		if (tree->left) {
			tree->left->left = createTree(3);
			tree->left->right = createTree(7);
		}
		if (tree->right) {
			tree->right->left = createTree(12);
			tree->right->right = createTree(20);
		}
	}
	tree = freeTree(tree);
	if (tree != NULL) {
		printf("\n  freeTree test1 PICAT\n");
		printf("  freeTree trebuie sa returneze NULL\n");
		passed = 0;
	}

	// Test 2: freeTree pe NULL
	if (passed) {
		tree = NULL;
		tree = freeTree(tree);
		if (tree != NULL) {
			printf("\n  freeTree test2 PICAT (arbore vid)\n");
			printf("  freeTree(NULL) trebuie sa returneze NULL\n");
			passed = 0;
		}
	}

	// Test 3: freeTree pe un singur nod
	if (passed) {
		tree = createTree(42);
		tree = freeTree(tree);
		if (tree != NULL) {
			printf("\n  freeTree test3 PICAT (un singur nod)\n");
			printf("  freeTree trebuie sa returneze NULL\n");
			passed = 0;
		}
	}

	// Test 4: apel dublu - dupa primul freeTree, al doilea pe NULL
	if (passed) {
		tree = createTree(99);
		tree = freeTree(tree);
		tree = freeTree(tree);
		if (tree != NULL) {
			printf("\n  freeTree test4 PICAT (apel dublu)\n");
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.25;
		printf("freeTree \t- Toate testele au trecut!\tPuncte: 0.25\n");
	} else {
		all_passed = 0;
		printf("freeTree \t- PICAT\n");
	}
	current_test[0] = '\0';
	return passed;
}

// ============================================================================

void run_all() {
	test_helpers();
	test_minimum_fn();
	test_rightRotation_fn();
	test_leftRotation_fn();
	test_insert_fn();
	test_delete_fn();
	test_freeTree_fn();
}

void run_selective(int argc, char **argv) {
	int i;
	int viz[8] = {0};
	for (i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		if (n < 1 || n > 7 || viz[n]) continue;
		viz[n] = 1;
		switch (n) {
			case 1: test_helpers(); break;
			case 2: test_minimum_fn(); break;
			case 3: test_rightRotation_fn(); break;
			case 4: test_leftRotation_fn(); break;
			case 5: test_insert_fn(); break;
			case 6: test_delete_fn(); break;
			case 7: test_freeTree_fn(); break;
		}
	}
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
