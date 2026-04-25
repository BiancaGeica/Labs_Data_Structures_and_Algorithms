#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OrderedSet are nevoie de T definit inainte de include.
typedef int T;
#include "OrderedSet.h"

#include "bintree.h"
#include "avl.h"

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
// Partea I: OrderedSet - contains
// =============================================================================
static char *test_osset_contains(void) {
    OrderedSet *s = createOrderedSet(8);
    int data[] = {1, 3, 5, 7, 9, 11, 13};
    for (int i = 0; i < 7; i++)
        s->elements[i] = data[i];
    s->size = 7;

    for (int i = 0; i < 7; i++) {
        int r = contains(s, data[i]);
        if (!r)
            printf("\n  contains(%d) = 0 (asteptat 1)\n", data[i]);
        sd_assert("osset_contains - Test 1 picat (element prezent negasit)", r == 1);
    }

    int missing[] = {0, 2, 4, 6, 8, 10, 12, 14};
    for (int i = 0; i < 8; i++) {
        int r = contains(s, missing[i]);
        if (r)
            printf("\n  contains(%d) = 1 (asteptat 0)\n", missing[i]);
        sd_assert("osset_contains - Test 2 picat (element absent gasit)", r == 0);
    }

    destroyOrderedSet(s);
    printf("osset_contains \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Partea II: OrderedSet - add
// =============================================================================
static char *test_osset_add(void) {
    OrderedSet *s = createOrderedSet(4);
    int input[]    = {5, 3, 8, 1, 6, 2, 10, 4, 9, 7};
    int n = 10;
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < n; i++)
        add(s, input[i]);

    if (s->size != n)
        printf("\n  dupa 10 add-uri: size = %ld (asteptat %d)\n", s->size, n);
    sd_assert("osset_add - Test 1 picat (size dupa add-uri)", s->size == n);

    for (int i = 0; i < n; i++) {
        if (s->elements[i] != expected[i])
            printf("\n  elements[%d] = %d (asteptat %d)\n", i, s->elements[i], expected[i]);
        sd_assert("osset_add - Test 2 picat (ordine gresita)",
                  s->elements[i] == expected[i]);
    }

    add(s, 5);
    sd_assert("osset_add - Test 3 picat (duplicat acceptat)", s->size == n);

    destroyOrderedSet(s);
    printf("osset_add \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Partea III: BinaryTree - mirror
// =============================================================================
static char *test_bintree_mirror(void) {
    BTNode *root = NULL;
    int vals[] = {5, 3, 7, 1, 4, 6, 9};
    for (int i = 0; i < 7; i++)
        root = btInsert(root, vals[i]);

    int before[7] = {0};
    int idx = 0;
    btInorderFill(root, before, &idx);
    int expectedBefore[] = {1, 3, 4, 5, 6, 7, 9};
    for (int i = 0; i < 7; i++)
        sd_assert("bintree_mirror - Test 1 picat (inorder initial)",
                  before[i] == expectedBefore[i]);

    btMirror(root);
    int after[7] = {0};
    idx = 0;
    btInorderFill(root, after, &idx);
    int expectedAfter[] = {9, 7, 6, 5, 4, 3, 1};
    for (int i = 0; i < 7; i++) {
        if (after[i] != expectedAfter[i])
            printf("\n  inorder[%d] dupa mirror = %d (asteptat %d)\n",
                   i, after[i], expectedAfter[i]);
        sd_assert("bintree_mirror - Test 2 picat",
                  after[i] == expectedAfter[i]);
    }

    btMirror(root);
    int twice[7] = {0};
    idx = 0;
    btInorderFill(root, twice, &idx);
    for (int i = 0; i < 7; i++)
        sd_assert("bintree_mirror - Test 3 picat (dublu mirror != original)",
                  twice[i] == expectedBefore[i]);

    btFreeTree(&root);
    printf("bintree_mirror \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Partea IV: AVL - leftRotation
// =============================================================================
static int verifyHeights(AvlTree root) {
    if (root == NULL)
        return -1;
    int lh = verifyHeights(root->left);
    int rh = verifyHeights(root->right);
    if (lh == -2 || rh == -2)
        return -2;
    int actual = 1 + avlMax(lh, rh);
    if (root->height != actual)
        return -2;
    return actual;
}

static char *test_avl_leftRotation(void) {
    AvlTree root = NULL;

    root = avlInsert(root, 1);
    root = avlInsert(root, 2);
    root = avlInsert(root, 3);

    sd_assert("avl_leftRotation - Test 1 picat (root value)", root->value == 2);
    sd_assert("avl_leftRotation - Test 2 picat (left value)",
              root->left != NULL && root->left->value == 1);
    sd_assert("avl_leftRotation - Test 3 picat (right value)",
              root->right != NULL && root->right->value == 3);

    if (root->height != 1)
        printf("\n  root->height = %d (asteptat 1)\n", root->height);
    sd_assert("avl_leftRotation - Test 4 picat (root height gresit)",
              root->height == 1);
    sd_assert("avl_leftRotation - Test 5 picat (left height gresit)",
              root->left->height == 0);
    sd_assert("avl_leftRotation - Test 6 picat (right height gresit)",
              root->right->height == 0);

    sd_assert("avl_leftRotation - Test 7 picat (heights inconsistente dupa rotatie)",
              verifyHeights(root) != -2);

    root = avlInsert(root, 4);
    root = avlInsert(root, 5);
    sd_assert("avl_leftRotation - Test 8 picat (heights inconsistente dupa 2 rotatii)",
              verifyHeights(root) != -2);

    sd_assert("avl_leftRotation - Test 9 picat (structura finala root)",
              root->value == 2 && root->height == 2);
    sd_assert("avl_leftRotation - Test 10 picat (structura finala root->right)",
              root->right->value == 4 && root->right->height == 1);

    avlFreeTree(root);
    printf("avl_leftRotation \t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_osset_contains,    5.00);
    sd_run_test(test_osset_add,         5.00);
    sd_run_test(test_bintree_mirror,    5.00);
    sd_run_test(test_avl_leftRotation, 10.00);
    return 0;
}

int main(void) {
    char *result = all_tests();
    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 4);
    printf("SCOR BugFix: %.2f / 25.00\n\n", total_score);

    return result != 0;
}
