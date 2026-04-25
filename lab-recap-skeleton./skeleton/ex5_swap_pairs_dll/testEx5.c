#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Item;
#include "DoubleLinkedList.h"
#include "swap_pairs.h"

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

// Helper: construieste o lista din vector.
static List *buildList(int *vals, int n) {
    List *l = createList();
    for (int i = 0; i < n; i++)
        insertAt(l, vals[i], i);
    return l;
}

// Helper: verifica continutul listei parcurgand inainte.
static int matchesForward(List *l, int *expected, int n) {
    ListNode *curr = l->first;
    int i = 0;
    while (curr != NULL && i < n) {
        if (curr->elem != expected[i])
            return 0;
        curr = curr->next;
        i++;
    }
    return (curr == NULL && i == n);
}

// Helper: verifica continutul listei parcurgand inapoi (din tail spre head).
static int matchesBackward(List *l, int *expected, int n) {
    ListNode *curr = l->last;
    int i = n - 1;
    while (curr != NULL && i >= 0) {
        if (curr->elem != expected[i])
            return 0;
        curr = curr->prev;
        i--;
    }
    return (curr == NULL && i == -1);
}

// Helper: verifica ca legaturile prev/next sunt coerente.
static int linksConsistent(List *l) {
    if (l == NULL) return 0;
    if (l->first == NULL) return l->last == NULL;
    if (l->first->prev != NULL) return 0;
    if (l->last->next != NULL) return 0;

    ListNode *curr = l->first;
    while (curr->next != NULL) {
        if (curr->next->prev != curr) return 0;
        curr = curr->next;
    }
    return (curr == l->last);
}

// =============================================================================
// Test: lista goala, un singur nod, apel pe NULL
// =============================================================================
static char *test_trivial(void) {
    List *l = createList();
    swapPairs(l);
    sd_assert("trivial - Test 1 picat (lista goala)",
              l->first == NULL && l->last == NULL);
    destroyList(l);

    int vals[] = {42};
    l = buildList(vals, 1);
    swapPairs(l);
    int expected[] = {42};
    sd_assert("trivial - Test 2 picat (un nod - continut)",
              matchesForward(l, expected, 1));
    sd_assert("trivial - Test 3 picat (un nod - first==last)",
              l->first == l->last);
    sd_assert("trivial - Test 4 picat (un nod - linkuri)",
              linksConsistent(l));
    destroyList(l);

    // Apel pe NULL nu trebuie sa strice nimic (nu crash).
    swapPairs(NULL);

    printf("trivial \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: exact doua noduri
// =============================================================================
static char *test_pair(void) {
    int vals[] = {1, 2};
    List *l = buildList(vals, 2);
    swapPairs(l);
    int expected[] = {2, 1};
    sd_assert("pair - Test 1 picat (continut inainte)",
              matchesForward(l, expected, 2));
    sd_assert("pair - Test 2 picat (continut inapoi)",
              matchesBackward(l, expected, 2));
    sd_assert("pair - Test 3 picat (first==2)", l->first->elem == 2);
    sd_assert("pair - Test 4 picat (last==1)", l->last->elem == 1);
    sd_assert("pair - Test 5 picat (linkuri)", linksConsistent(l));
    destroyList(l);

    printf("pair \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: lungime para
// =============================================================================
static char *test_even(void) {
    int vals[] = {1, 2, 3, 4, 5, 6};
    List *l = buildList(vals, 6);
    swapPairs(l);
    int expected[] = {2, 1, 4, 3, 6, 5};
    if (!matchesForward(l, expected, 6)) {
        printf("\n  dupa swap, lista inainte = ");
        ListNode *it = l->first;
        while (it) { printf("%d ", it->elem); it = it->next; }
        printf("\n");
    }
    sd_assert("even - Test 1 picat (continut inainte)",
              matchesForward(l, expected, 6));
    sd_assert("even - Test 2 picat (continut inapoi)",
              matchesBackward(l, expected, 6));
    sd_assert("even - Test 3 picat (first==2)", l->first->elem == 2);
    sd_assert("even - Test 4 picat (last==5)", l->last->elem == 5);
    sd_assert("even - Test 5 picat (linkuri)", linksConsistent(l));
    destroyList(l);

    printf("even \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: lungime impara (ultimul nod ramane singur)
// =============================================================================
static char *test_odd(void) {
    int vals[] = {1, 2, 3, 4, 5};
    List *l = buildList(vals, 5);
    swapPairs(l);
    int expected[] = {2, 1, 4, 3, 5};
    if (!matchesForward(l, expected, 5)) {
        printf("\n  dupa swap, lista inainte = ");
        ListNode *it = l->first;
        while (it) { printf("%d ", it->elem); it = it->next; }
        printf("\n");
    }
    sd_assert("odd - Test 1 picat (continut inainte)",
              matchesForward(l, expected, 5));
    sd_assert("odd - Test 2 picat (continut inapoi)",
              matchesBackward(l, expected, 5));
    sd_assert("odd - Test 3 picat (first==2)", l->first->elem == 2);
    sd_assert("odd - Test 4 picat (last==5)", l->last->elem == 5);
    sd_assert("odd - Test 5 picat (linkuri)", linksConsistent(l));
    destroyList(l);

    int vals2[] = {7, 8, 9};
    l = buildList(vals2, 3);
    swapPairs(l);
    int expected2[] = {8, 7, 9};
    sd_assert("odd - Test 6 picat (3 elem - continut)",
              matchesForward(l, expected2, 3));
    sd_assert("odd - Test 7 picat (3 elem - linkuri)", linksConsistent(l));
    destroyList(l);

    printf("odd \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: dublu swap - dupa 2 apeluri succesive, lista revine la original
// =============================================================================
static char *test_double_swap(void) {
    int vals[] = {10, 20, 30, 40, 50, 60, 70, 80};
    List *l = buildList(vals, 8);
    swapPairs(l);
    swapPairs(l);
    sd_assert("double_swap - Test 1 picat (nu s-a revenit la original)",
              matchesForward(l, vals, 8));
    sd_assert("double_swap - Test 2 picat (inapoi)",
              matchesBackward(l, vals, 8));
    sd_assert("double_swap - Test 3 picat (linkuri)", linksConsistent(l));
    destroyList(l);

    printf("double_swap \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: lista lunga (stress)
// =============================================================================
static char *test_long(void) {
    int N = 100;
    int *vals = (int *)malloc(N * sizeof(int));
    int *expected = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) vals[i] = i + 1;

    for (int i = 0; i + 1 < N; i += 2) {
        expected[i]     = vals[i + 1];
        expected[i + 1] = vals[i];
    }
    if (N % 2 == 1) expected[N - 1] = vals[N - 1];

    List *l = buildList(vals, N);
    swapPairs(l);
    sd_assert("long - Test 1 picat (continut inainte)",
              matchesForward(l, expected, N));
    sd_assert("long - Test 2 picat (continut inapoi)",
              matchesBackward(l, expected, N));
    sd_assert("long - Test 3 picat (linkuri)", linksConsistent(l));

    destroyList(l);
    free(vals);
    free(expected);

    printf("long \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_trivial,     2.00);
    sd_run_test(test_pair,        2.00);
    sd_run_test(test_even,        3.00);
    sd_run_test(test_odd,         3.00);
    sd_run_test(test_double_swap, 2.00);
    sd_run_test(test_long,        3.00);
    return 0;
}

int main(void) {
    char *result = all_tests();
    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 6);
    printf("SCOR SwapPairs: %.2f / 15.00\n\n", total_score);

    return result != 0;
}
