#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tree.h"

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

// =============================================================================
// Helper: construieste un BST din valorile date in vector.
// =============================================================================
static Tree buildBST(int *vals, int n) {
    Tree root = NULL;
    for (int i = 0; i < n; i++)
        root = bstInsert(root, vals[i]);
    return root;
}

// =============================================================================
// Test: isBST pe BST-uri valide
// =============================================================================
static char *test_isBST_valid(void) {
    // Arbore vid = BST valid
    sd_assert("isBST_valid - Test 1 picat (arbore vid)", isBST(NULL) == 1);

    // Singur nod
    Tree single = bstCreate(42);
    sd_assert("isBST_valid - Test 2 picat (un nod)", isBST(single) == 1);
    bstDestroy(&single);

    // BST "clasic" cu mai multe valori
    int vals1[] = {5, 3, 7, 1, 4, 6, 9};
    Tree t1 = buildBST(vals1, 7);
    sd_assert("isBST_valid - Test 3 picat (BST clasic)", isBST(t1) == 1);
    bstDestroy(&t1);

    // BST degenerat la dreapta (lant sortat crescator)
    int vals2[] = {1, 2, 3, 4, 5};
    Tree t2 = buildBST(vals2, 5);
    sd_assert("isBST_valid - Test 4 picat (lant crescator)", isBST(t2) == 1);
    bstDestroy(&t2);

    // BST degenerat la stanga
    int vals3[] = {5, 4, 3, 2, 1};
    Tree t3 = buildBST(vals3, 5);
    sd_assert("isBST_valid - Test 5 picat (lant descrescator)", isBST(t3) == 1);
    bstDestroy(&t3);

    printf("isBST_valid \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: isBST pe arbori INVALIZI (construiti manual)
// =============================================================================
static char *test_isBST_invalid(void) {
    // Arbore construit manual, stricat intentionat:
    //   radacina 5, fiu stang 3 (cu copii 1 si 8), fiu drept 7.
    //   8 este in subarborele stang al lui 5, dar 8 > 5 - incalca BST!
    Tree root = bstCreate(5);
    root->left = bstCreate(3);
    root->right = bstCreate(7);
    root->left->left = bstCreate(1);
    root->left->right = bstCreate(8);

    int r = isBST(root);
    if (r != 0)
        printf("\n  isBST(arbore invalid #1) = %d (asteptat 0)\n", r);
    sd_assert("isBST_invalid - Test 1 picat", r == 0);
    bstDestroy(&root);

    // Arbore cu dublura: 5 / (3 dr=5) - dublura la radacina
    root = bstCreate(5);
    root->left = bstCreate(3);
    root->left->right = bstCreate(5);
    sd_assert("isBST_invalid - Test 2 picat (dublura)", isBST(root) == 0);
    bstDestroy(&root);

    // Arbore cu nod stang mai mare
    root = bstCreate(10);
    root->left = bstCreate(15);
    sd_assert("isBST_invalid - Test 3 picat (stang > radacina)", isBST(root) == 0);
    bstDestroy(&root);

    // Arbore cu nod drept mai mic
    root = bstCreate(10);
    root->right = bstCreate(5);
    sd_assert("isBST_invalid - Test 4 picat (drept < radacina)", isBST(root) == 0);
    bstDestroy(&root);

    printf("isBST_invalid \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: kthSmallest pe BST corect
// =============================================================================
static char *test_kthSmallest_basic(void) {
    int vals[] = {5, 3, 7, 1, 4, 6, 9};
    Tree t = buildBST(vals, 7);
    int sortedExpected[] = {1, 3, 4, 5, 6, 7, 9};

    for (int k = 1; k <= 7; k++) {
        int r = kthSmallest(t, k);
        if (r != sortedExpected[k - 1])
            printf("\n  kthSmallest(k=%d) = %d (asteptat %d)\n",
                   k, r, sortedExpected[k - 1]);
        sd_assert("kthSmallest_basic - Test picat", r == sortedExpected[k - 1]);
    }

    bstDestroy(&t);
    printf("kthSmallest_basic \t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: kthSmallest pe lant degenerat (left-only si right-only)
// =============================================================================
static char *test_kthSmallest_chain(void) {
    // Lant crescator: 1,2,3,4,5 - totul pe dreapta
    int ascending[] = {1, 2, 3, 4, 5};
    Tree t = buildBST(ascending, 5);
    for (int k = 1; k <= 5; k++) {
        int r = kthSmallest(t, k);
        sd_assert("kthSmallest_chain - Test 1 picat (crescator)", r == k);
    }
    bstDestroy(&t);

    // Lant descrescator: 5,4,3,2,1 - totul pe stanga
    int descending[] = {5, 4, 3, 2, 1};
    t = buildBST(descending, 5);
    for (int k = 1; k <= 5; k++) {
        int r = kthSmallest(t, k);
        sd_assert("kthSmallest_chain - Test 2 picat (descrescator)", r == k);
    }
    bstDestroy(&t);

    printf("kthSmallest_chain \t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: kthSmallest pe input invalid
// =============================================================================
static char *test_kthSmallest_invalid(void) {
    // Arbore vid
    sd_assert("kthSmallest_invalid - Test 1 picat (arbore vid)",
              kthSmallest(NULL, 1) == INT_MIN);

    // k <= 0
    Tree t = bstCreate(42);
    sd_assert("kthSmallest_invalid - Test 2 picat (k=0)",
              kthSmallest(t, 0) == INT_MIN);
    sd_assert("kthSmallest_invalid - Test 3 picat (k=-5)",
              kthSmallest(t, -5) == INT_MIN);

    // k > size
    sd_assert("kthSmallest_invalid - Test 4 picat (k=2 pe arbore cu 1 nod)",
              kthSmallest(t, 2) == INT_MIN);
    bstDestroy(&t);

    int vals[] = {5, 3, 7};
    t = buildBST(vals, 3);
    sd_assert("kthSmallest_invalid - Test 5 picat (k=100)",
              kthSmallest(t, 100) == INT_MIN);
    bstDestroy(&t);

    printf("kthSmallest_invalid \t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_isBST_valid,          3.00);
    sd_run_test(test_isBST_invalid,        4.00);
    sd_run_test(test_kthSmallest_basic,    4.00);
    sd_run_test(test_kthSmallest_chain,    2.00);
    sd_run_test(test_kthSmallest_invalid,  2.00);
    return 0;
}

int main(void) {
    char *result = all_tests();
    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 5);
    printf("SCOR BSTUtils: %.2f / 15.00\n\n", total_score);

    return result != 0;
}
