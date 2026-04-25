#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int T;
#include "SortedList.h"

typedef SLNode *Item;
#include "Heap.h"

#include "merge_k.h"

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
// Helper: construieste o lista sortata din vector (presupus sortat deja)
static SortedList buildSorted(int *vals, int n) {
    SortedList l = NULL;
    for (int i = n - 1; i >= 0; i--)
        l = slInsert(l, vals[i]);
    return l;
}

static int checkSorted(SortedList l, int *expected, int n) {
    int i = 0;
    while (l != NULL && i < n) {
        if (l->value != expected[i])
            return 0;
        l = l->next;
        i++;
    }
    return (l == NULL && i == n);
}

// =============================================================================
// Test: K = 0 sau lists = NULL -> rezultat NULL
// =============================================================================
static char *test_empty_input(void) {
    SortedList r = mergeKSortedLists(NULL, 0);
    sd_assert("empty_input - Test 1 picat", r == NULL);

    r = mergeKSortedLists(NULL, 5);
    sd_assert("empty_input - Test 2 picat", r == NULL);

    // K liste, toate NULL
    SortedList lists[3] = {NULL, NULL, NULL};
    r = mergeKSortedLists(lists, 3);
    sd_assert("empty_input - Test 3 picat (toate NULL)", r == NULL);

    printf("empty_input \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: K = 1 - intoarce o copie identica
// =============================================================================
static char *test_single_list(void) {
    int vals[] = {1, 3, 5, 7, 9};
    SortedList l = buildSorted(vals, 5);
    SortedList lists[1] = {l};

    SortedList r = mergeKSortedLists(lists, 1);
    sd_assert("single_list - Test 1 picat (continut gresit)",
              checkSorted(r, vals, 5));

    // Rezultatul NU trebuie sa fie aceeasi lista (noduri noi)
    sd_assert("single_list - Test 2 picat (acelasi nod reutilizat)", r != l);
    sd_assert("single_list - Test 3 picat (al doilea nod reutilizat)",
              r->next != l->next);

    slFree(l);
    slFree(r);

    printf("single_list \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: K = 2 cu liste de lungimi diferite
// =============================================================================
static char *test_two_lists(void) {
    int a[] = {1, 4, 7};
    int b[] = {2, 3, 5, 6, 8, 9, 10};
    SortedList la = buildSorted(a, 3);
    SortedList lb = buildSorted(b, 7);
    SortedList lists[2] = {la, lb};

    SortedList r = mergeKSortedLists(lists, 2);
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sd_assert("two_lists - Test 1 picat", checkSorted(r, expected, 10));

    slFree(la);
    slFree(lb);
    slFree(r);

    printf("two_lists \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: K = 4 cu valori care se suprapun (dublete intre liste)
// =============================================================================
static char *test_four_lists(void) {
    int a[] = {1, 5, 9};
    int b[] = {2, 5, 10};
    int c[] = {3, 6, 11};
    int d[] = {4, 7, 12};
    SortedList la = buildSorted(a, 3);
    SortedList lb = buildSorted(b, 3);
    SortedList lc = buildSorted(c, 3);
    SortedList ld = buildSorted(d, 3);
    SortedList lists[4] = {la, lb, lc, ld};

    SortedList r = mergeKSortedLists(lists, 4);
    int expected[] = {1, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12};
    if (!checkSorted(r, expected, 12)) {
        printf("\n  merge K=4: continut gresit. Rezultat actual: ");
        SortedList it = r;
        while (it != NULL) {
            printf("%d ", it->value);
            it = it->next;
        }
        printf("\n");
    }
    sd_assert("four_lists - Test 1 picat", checkSorted(r, expected, 12));

    slFree(la); slFree(lb); slFree(lc); slFree(ld);
    slFree(r);

    printf("four_lists \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: K = 3 cu o lista goala la mijloc (must handle gracefully)
// =============================================================================
static char *test_with_null(void) {
    int a[] = {1, 2, 3};
    int c[] = {4, 5, 6};
    SortedList la = buildSorted(a, 3);
    SortedList lc = buildSorted(c, 3);
    SortedList lists[3] = {la, NULL, lc};

    SortedList r = mergeKSortedLists(lists, 3);
    int expected[] = {1, 2, 3, 4, 5, 6};
    sd_assert("with_null - Test 1 picat", checkSorted(r, expected, 6));

    slFree(la);
    slFree(lc);
    slFree(r);

    printf("with_null \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: K mare cu liste de lungime 1 (stress pe heap)
// =============================================================================
static char *test_many_singles(void) {
    int K = 20;
    SortedList lists[20];
    for (int i = 0; i < K; i++) {
        int v = K - 1 - i;  // valori 19, 18, ..., 0
        lists[i] = slCreate(v);
    }

    SortedList r = mergeKSortedLists(lists, K);
    // Rezultatul trebuie sa fie 0, 1, 2, ..., 19
    SortedList it = r;
    for (int expected = 0; expected < K; expected++) {
        if (it == NULL)
            return "many_singles - Test 1 picat (lista mai scurta)";
        if (it->value != expected)
            return "many_singles - Test 2 picat (valoare gresita)";
        it = it->next;
    }
    sd_assert("many_singles - Test 3 picat (lista mai lunga)", it == NULL);

    for (int i = 0; i < K; i++)
        slFree(lists[i]);
    slFree(r);

    printf("many_singles \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_empty_input,   2.00);
    sd_run_test(test_single_list,   2.00);
    sd_run_test(test_two_lists,     3.00);
    sd_run_test(test_four_lists,    3.00);
    sd_run_test(test_with_null,     2.00);
    sd_run_test(test_many_singles,  3.00);
    return 0;
}

int main(void) {
    char *result = all_tests();
    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 6);
    printf("SCOR MergeK: %.2f / 15.00\n\n", total_score);

    return result != 0;
}
