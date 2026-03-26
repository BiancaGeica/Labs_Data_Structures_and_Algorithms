/*
*	Created by Nan Mihai on 28.03.2021
*	Copyright (c) 2021 Nan Mihai. All rights reserved.
*	Laborator 5 - Structuri de date
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "tree.h"

float total_score = 0;
int all_passed = 1;
int has_graphviz = 0;

int test1[] = {9, 7, 5, 9, 4, 10, 6, 8, -8, 12};
int test2[] = {11, 9, 7, 25, 12, 29, 27, 8, 5, 1, 35, 26};
int test3[] = {26, 50, 150, 35, 175, 155, 100, 95, 9, 15, 45, 40, 4, 47, 98, \
	97, 99, 90, 200, 50, 68, 35, 39, 37, 44, 43, 46};

int postorder1[] = {-8, 4, 6, 5, 8, 12, 10, 9, 7};
int postorder2[] = {1, 5, 8, 7, 12, 26, 27, 35, 29, 25, 9};
int postorder3[] = {4, 15, 9, 37, 39, 43, 44, 40, 46, 47, 45, 35, 68, 90, 97, 99, 98, 95, 100, 155, 200, 175, 150, 50};

int preorder1[] = {7, 5, 4, -8, 6, 9, 8, 10, 12};
int preorder2[] = {9, 7, 5, 1, 8, 25, 12, 29, 27, 26, 35};
int preorder3[] = {50, 35, 9, 4, 15, 45, 40, 39, 37, 44, 43, 47, 46, 150, 100, 95, 90, 68, 98, 97, 99, 175, 155, 200};

int inorder1[] = {-8, 4, 5, 6, 7, 8, 9, 10, 12};
int inorder2[] = {1, 5, 7, 8, 9, 12, 25, 26, 27, 29, 35};
int inorder3[] = {4, 9, 15, 35, 37, 39, 40, 43, 44, 45, 46, 47, 50, 68, 90, 95, 97, 98, 99, 100, 150, 155, 175, 200};

// ============================================================================
// Utilitare
// ============================================================================

int checkBST(Tree node) {
	if (node == NULL) return 1;
	if (node->left != NULL && node->left->value > node->value) return 0;
	if (node->right != NULL && node->right->value < node->value) return 0;
	if (!checkBST(node->left) || !checkBST(node->right)) return 0;
	return 1;
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

int collectIdx;
int collectBuf[200];

void collectInorder(Tree root) {
	if (!root) return;
	collectInorder(root->left);
	collectBuf[collectIdx++] = root->value;
	collectInorder(root->right);
}
void collectPreorder(Tree root) {
	if (!root) return;
	collectBuf[collectIdx++] = root->value;
	collectPreorder(root->left);
	collectPreorder(root->right);
}
void collectPostorder(Tree root) {
	if (!root) return;
	collectPostorder(root->left);
	collectPostorder(root->right);
	collectBuf[collectIdx++] = root->value;
}

int checkTraversal(int *expected, int expectedSize, int *got, int gotSize) {
	if (expectedSize != gotSize) return 0;
	int i;
	for (i = 0; i < expectedSize; i++)
		if (expected[i] != got[i]) return 0;
	return 1;
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
	system("rm -f tree*.png mirror*.png 2>/dev/null");
}

// ============================================================================
// Teste - fiecare returneaza 1 daca a trecut, 0 daca a picat
// ============================================================================

int test_insert() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1", "test2", "test3"};
	char pngName[32];
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		int ok = (tree != NULL && checkBST(tree));
		snprintf(pngName, sizeof(pngName), "tree%d.png", t + 1);
		generatePNG(tree, pngName, ok ? "green" : "orange");
		if (!ok) {
			printf("\n  insert %s PICAT\n", names[t]);
			printf("  Input (ordinea inserarii): "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  Arborele rezultat nu respecta proprietatea BST\n");
			passed = 0;
			if (tree != NULL) freeTree(&tree);
			break;
		}
		if (tree != NULL) freeTree(&tree);
	}

	if (passed) {
		total_score += 1.5;
		if (has_graphviz)
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 1.50\t(vezi tree1/2/3.png)\n");
		else
			printf("insert \t\t- Toate testele au trecut!\tPuncte: 1.50\n");
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

typedef void (*PrintTraversal)(Tree);

// Capteaza output-ul unei functii print* a studentului intr-un buffer
// si parseaza numerele in collectBuf
int captureTraversal(PrintTraversal printFn, Tree tree) {
	// Redirectam stdout catre un fisier temporar
	fflush(stdout);
	FILE *tmp = tmpfile();
	int saved_stdout = dup(fileno(stdout));
	dup2(fileno(tmp), fileno(stdout));

	// Apelam functia studentului
	printFn(tree);
	fflush(stdout);

	// Restauram stdout
	dup2(saved_stdout, fileno(stdout));
	close(saved_stdout);

	// Citim ce a scris studentul
	rewind(tmp);
	collectIdx = 0;
	int val;
	while (fscanf(tmp, "%d", &val) == 1) {
		collectBuf[collectIdx++] = val;
	}
	fclose(tmp);
	return collectIdx;
}

int test_traversal(const char *name, PrintTraversal printFn,
	int *exp1, int sz1, int *exp2, int sz2, int *exp3, int sz3, float score) {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	int *expected[] = {exp1, exp2, exp3};
	int sizes[] = {sz1, sz2, sz3};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		captureTraversal(printFn, tree);
		int ok = checkTraversal(expected[t], sizes[t], collectBuf, collectIdx);
		if (!ok) {
			printf("\n  %s %s PICAT\n", name, names[t]);
			printf("  Input (ordinea inserarii): "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  Asteptat: "); printArrayInline(expected[t], sizes[t]); printf("\n");
			printf("  Primit:   "); printArrayInline(collectBuf, collectIdx); printf("\n");
			passed = 0;
			freeTree(&tree);
			break;
		}
		freeTree(&tree);
	}

	if (passed) {
		total_score += score;
		printf("%s \t- Toate testele au trecut!\tPuncte: %.2f\n", name, score);
	} else {
		all_passed = 0;
		printf("%s \t- PICAT\n", name);
	}
	return passed;
}

// ============================================================================

// Numara nodurile accesibile din arbore (independent de size() studentului)
int countNodes(Tree root) {
	if (root == NULL) return 0;
	return 1 + countNodes(root->left) + countNodes(root->right);
}

int test_free() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		int totalNodes = countNodes(tree);
		freeTree(&tree);

		if (tree != NULL) {
			printf("\n  freeTree %s PICAT\n", names[t]);
			printf("  Arborele avea %d noduri\n", totalNodes);
			printf("  Dupa freeTree, root trebuie sa fie NULL, dar nu este\n");
			passed = 0;
		}
	}

	if (passed) {
		total_score += 0.75;
		printf("freeTree \t- root == NULL? DA\t\tPuncte: 0.75\n");
	} else {
		all_passed = 0;
		printf("freeTree \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_size() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	int expectedSizes[] = {test1[0], test2[0], test3[0] - 2};
	char *names[] = {"test1", "test2", "test3 (2 duplicate: 50 si 35)"};
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		if (size(tree) != expectedSizes[t]) {
			printf("\n  size %s PICAT\n", names[t]);
			printf("  Input (ordinea inserarii): "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  size() -> asteptat: %d, primit: %d\n", expectedSizes[t], size(tree));
			passed = 0;
			freeTree(&tree);
			break;
		}
		freeTree(&tree);
	}

	if (passed) {
		total_score += 0.75;
		printf("size \t\t- Toate testele au trecut!\tPuncte: 0.75\n");
	} else {
		all_passed = 0;
		printf("size \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_maxDepth() {
	int passed = 1;
	Tree tree;
	int *tests[] = {test1, test2, test3};
	int expectedDepths[] = {3, 4, 5};
	char *names[] = {"test1", "test2", "test3"};
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		if (maxDepth(tree) != expectedDepths[t]) {
			printf("\n  maxDepth %s PICAT\n", names[t]);
			printf("  Input (ordinea inserarii): "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  Adancimea = nivel maxim (radacina = nivel 0)\n");
			printf("  maxDepth() -> asteptat: %d, primit: %d\n", expectedDepths[t], maxDepth(tree));
			passed = 0;
			freeTree(&tree);
			break;
		}
		freeTree(&tree);
	}

	if (passed) {
		total_score += 0.75;
		printf("maxDepth \t- Toate testele au trecut!\tPuncte: 0.75\n");
	} else {
		all_passed = 0;
		printf("maxDepth \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int areMirrorTrees(Tree root1, Tree root2) {
	if (root1 == NULL && root2 == NULL) return 1;
	if (root1 == NULL || root2 == NULL) return 0;
	if (root1->value != root2->value) return 0;
	return areMirrorTrees(root1->left, root2->right)
		&& areMirrorTrees(root1->right, root2->left);
}

int test_mirror() {
	int passed = 1;
	Tree tree, original;
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1", "test2", "test3"};
	char pngName[32];
	int t;

	for (t = 0; t < 3; t++) {
		tree = buildTree(tests[t]);
		original = buildTree(tests[t]);
		mirror(tree);
		int ok = (tree != NULL && areMirrorTrees(tree, original));
		snprintf(pngName, sizeof(pngName), "mirror%d.png", t + 1);
		generatePNG(tree, pngName, ok ? "red" : "orange");
		if (!ok) {
			printf("\n  mirror %s PICAT\n", names[t]);
			printf("  Input (ordinea inserarii): "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  Arborele oglindit nu corespunde originalului\n");
			passed = 0;
			freeTree(&tree);
			freeTree(&original);
			break;
		}
		freeTree(&tree);
		freeTree(&original);
	}

	if (passed) {
		total_score += 2.0;
		if (has_graphviz)
			printf("mirror \t\t- Toate testele au trecut!\tPuncte: 2.00\t(vezi mirror1/2/3.png)\n");
		else
			printf("mirror \t\t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		if (has_graphviz)
			printf("mirror \t\t- PICAT\t\t\t\t\t\t\t(vezi mirror*.png)\n");
		else
			printf("mirror \t\t- PICAT\n");
	}
	return passed;
}

// ============================================================================

int test_sameTree() {
	int passed = 1;
	Tree tree, copy;

	// Test 1-3: arbori identici
	int *tests[] = {test1, test2, test3};
	char *names[] = {"test1 (identici)", "test2 (identici)", "test3 (identici, cu duplicate)"};
	int t;

	for (t = 0; t < 3 && passed; t++) {
		tree = buildTree(tests[t]);
		copy = buildTree(tests[t]);
		if (!sameTree(tree, copy)) {
			printf("\n  sameTree %s PICAT\n", names[t]);
			printf("  Input: "); printArrayInline(tests[t] + 1, tests[t][0]); printf("\n");
			printf("  sameTree() -> asteptat: 1, primit: 0\n");
			passed = 0;
		}
		freeTree(&tree);
		freeTree(&copy);
	}

	// Test 4: arbori diferiti
	if (passed) {
		tree = buildTree(test1);
		copy = buildTree(test2);
		if (sameTree(tree, copy)) {
			printf("\n  sameTree test4 PICAT (arbori diferiti)\n");
			printf("  Arbore 1: "); printArrayInline(test1 + 1, test1[0]); printf("\n");
			printf("  Arbore 2: "); printArrayInline(test2 + 1, test2[0]); printf("\n");
			printf("  sameTree() -> asteptat: 0, primit: 1\n");
			passed = 0;
		}
		freeTree(&tree);
		freeTree(&copy);
	}

	// Test 5: un arbore NULL
	if (passed) {
		tree = buildTree(test1);
		if (sameTree(tree, NULL)) {
			printf("\n  sameTree test5 PICAT (un arbore NULL)\n");
			printf("  sameTree(arbore, NULL) -> asteptat: 0, primit: 1\n");
			passed = 0;
		}
		freeTree(&tree);
	}

	// Test 6: ambii NULL
	if (passed) {
		if (!sameTree(NULL, NULL)) {
			printf("\n  sameTree test6 PICAT (ambii NULL)\n");
			printf("  sameTree(NULL, NULL) -> asteptat: 1, primit: 0\n");
			passed = 0;
		}
	}

	if (passed) {
		total_score += 2.0;
		printf("sameTree \t- Toate testele au trecut!\tPuncte: 2.00\n");
	} else {
		all_passed = 0;
		printf("sameTree \t- PICAT\n");
	}
	return passed;
}

// ============================================================================

void run_all() {
	test_insert();
	test_traversal("postorder", printPostorder, postorder1, 9, postorder2, 11, postorder3, 24, 0.75);
	test_traversal("preorder ", printPreorder, preorder1, 9, preorder2, 11, preorder3, 24, 0.75);
	test_traversal("inorder  ", printInorder, inorder1, 9, inorder2, 11, inorder3, 24, 0.75);
	test_size();
	test_maxDepth();
	test_mirror();
	test_sameTree();
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
			case 2: test_traversal("postorder", printPostorder, postorder1, 9, postorder2, 11, postorder3, 24, 0.75); break;
			case 3: test_traversal("preorder ", printPreorder, preorder1, 9, preorder2, 11, preorder3, 24, 0.75); break;
			case 4: test_traversal("inorder  ", printInorder, inorder1, 9, inorder2, 11, inorder3, 24, 0.75); break;
			case 5: test_size(); break;
			case 6: test_maxDepth(); break;
			case 7: test_mirror(); break;
			case 8: test_sameTree(); break;
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
