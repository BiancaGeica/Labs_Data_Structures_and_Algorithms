#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hot_potato.h"

// -----------------------------------------------------------------------------
// Macros (stil laboratoare 04-08)
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
// Test: N=1 - un singur jucator (castigatorul e el insusi, fara eliminari)
// =============================================================================
static char *test_single_player(void) {
    int elim[1] = {-1};
    int winner = hotPotato(1, 3, elim);
    if (winner != 1)
        printf("\n  hotPotato(1, 3): asteptat castigator=1, primit %d\n", winner);
    sd_assert("single_player - Test 1 picat", winner == 1);
    printf("single_player \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: k=1 - primul jucator al rundei este eliminat direct (fara pasari)
// Pentru N=5, k=1: ordinea eliminarilor este 1, 2, 3, 4; castigator 5.
// =============================================================================
static char *test_k_one(void) {
    int elim[4] = {0};
    int winner = hotPotato(5, 1, elim);
    int expected[4] = {1, 2, 3, 4};
    if (winner != 5)
        printf("\n  hotPotato(5, 1): asteptat 5, primit %d\n", winner);
    sd_assert("k_one - Test 1 picat (castigator)", winner == 5);
    for (int i = 0; i < 4; i++) {
        if (elim[i] != expected[i])
            printf("\n  elim[%d] = %d, asteptat %d\n", i, elim[i], expected[i]);
        sd_assert("k_one - Test 2 picat (ordinea eliminarilor)", elim[i] == expected[i]);
    }
    printf("k_one \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: N=5, k=2 - castigatorul e 3, ordine eliminari: 2, 4, 1, 5
// =============================================================================
static char *test_classic(void) {
    int elim[4] = {0};
    int winner = hotPotato(5, 2, elim);
    int expected[4] = {2, 4, 1, 5};
    if (winner != 3)
        printf("\n  hotPotato(5, 2): asteptat 3, primit %d\n", winner);
    sd_assert("classic - Test 1 picat (castigator)", winner == 3);
    for (int i = 0; i < 4; i++) {
        if (elim[i] != expected[i])
            printf("\n  elim[%d] = %d, asteptat %d\n", i, elim[i], expected[i]);
        sd_assert("classic - Test 2 picat (ordinea eliminarilor)", elim[i] == expected[i]);
    }
    printf("classic \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: N=7, k=3 - ordine cunoscuta: 3, 6, 2, 7, 5, 1; castigator 4
// =============================================================================
static char *test_seven_three(void) {
    int elim[6] = {0};
    int winner = hotPotato(7, 3, elim);
    int expected[6] = {3, 6, 2, 7, 5, 1};
    if (winner != 4)
        printf("\n  hotPotato(7, 3): asteptat 4, primit %d\n", winner);
    sd_assert("seven_three - Test 1 picat (castigator)", winner == 4);
    for (int i = 0; i < 6; i++) {
        if (elim[i] != expected[i])
            printf("\n  elim[%d] = %d, asteptat %d\n", i, elim[i], expected[i]);
        sd_assert("seven_three - Test 2 picat", elim[i] == expected[i]);
    }
    printf("seven_three \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: N mare si k mare - verificare stabilitate + memory-safety
// =============================================================================
static char *test_large(void) {
    int N = 100, k = 7;
    int *elim = (int *)malloc((N - 1) * sizeof(int));
    int winner = hotPotato(N, k, elim);
    sd_assert("large - Test 1 picat (castigator valid)", winner >= 1 && winner <= N);
    // Sanity: toti jucatorii eliminati sunt diferiti si nu apare castigatorul
    for (int i = 0; i < N - 1; i++) {
        sd_assert("large - Test 2 picat (id invalid in eliminari)",
                  elim[i] >= 1 && elim[i] <= N);
        sd_assert("large - Test 3 picat (castigatorul e in eliminari)",
                  elim[i] != winner);
        for (int j = i + 1; j < N - 1; j++)
            sd_assert("large - Test 4 picat (duplicat in eliminari)",
                      elim[i] != elim[j]);
    }
    free(elim);
    printf("large \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_single_player, 2.00);
    sd_run_test(test_k_one,         2.50);
    sd_run_test(test_classic,       3.00);
    sd_run_test(test_seven_three,   3.00);
    sd_run_test(test_large,         4.50);
    return 0;
}

int main(void) {
    char *result = all_tests();

    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 5);
    printf("SCOR HotPotato: %.2f / 15.00\n\n", total_score);

    return result != 0;
}
