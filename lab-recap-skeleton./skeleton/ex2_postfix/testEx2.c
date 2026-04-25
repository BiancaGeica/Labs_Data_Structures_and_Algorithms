#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "postfix.h"

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
// Test: expresii simple (un singur operator)
// =============================================================================
static char *test_simple(void) {
    long r;
    r = evaluatePostfix("3 4 +");
    if (r != 7) printf("\n  \"3 4 +\" -> %ld (asteptat 7)\n", r);
    sd_assert("simple - Test 1 picat", r == 7);

    r = evaluatePostfix("10 3 -");
    sd_assert("simple - Test 2 picat", r == 7);

    r = evaluatePostfix("5 6 *");
    sd_assert("simple - Test 3 picat", r == 30);

    r = evaluatePostfix("20 4 /");
    sd_assert("simple - Test 4 picat", r == 5);

    printf("simple \t\t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: expresii cu mai multi operatori
// =============================================================================
static char *test_compound(void) {
    long r;
    // "12 3 + 5 *" = (12+3)*5 = 75
    r = evaluatePostfix("12 3 + 5 *");
    if (r != 75) printf("\n  \"12 3 + 5 *\" -> %ld (asteptat 75)\n", r);
    sd_assert("compound - Test 1 picat", r == 75);

    // "7 8 * 2 +" = 56 + 2 = 58
    r = evaluatePostfix("7 8 * 2 +");
    sd_assert("compound - Test 2 picat", r == 58);

    // "5 1 2 + 4 * + 3 -" = 5 + (1+2)*4 - 3 = 5 + 12 - 3 = 14
    r = evaluatePostfix("5 1 2 + 4 * + 3 -");
    if (r != 14) printf("\n  \"5 1 2 + 4 * + 3 -\" -> %ld (asteptat 14)\n", r);
    sd_assert("compound - Test 3 picat", r == 14);

    printf("compound \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: operanzi cu mai multe cifre
// =============================================================================
static char *test_multidigit(void) {
    long r;
    r = evaluatePostfix("100 200 +");
    sd_assert("multidigit - Test 1 picat", r == 300);

    r = evaluatePostfix("1234 2 *");
    sd_assert("multidigit - Test 2 picat", r == 2468);

    r = evaluatePostfix("999 999 *");
    sd_assert("multidigit - Test 3 picat", r == 998001L);

    r = evaluatePostfix("1000000 1000000 *");
    sd_assert("multidigit - Test 4 picat", r == 1000000000000L);

    printf("multidigit \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: un singur operand (expresie valida)
// =============================================================================
static char *test_single_operand(void) {
    long r;
    r = evaluatePostfix("42");
    sd_assert("single_operand - Test 1 picat", r == 42);

    r = evaluatePostfix("0");
    sd_assert("single_operand - Test 2 picat", r == 0);

    r = evaluatePostfix("   123   ");
    sd_assert("single_operand - Test 3 picat", r == 123);

    printf("single_operand \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
// Test: expresii invalide -> LONG_MIN
// =============================================================================
static char *test_invalid(void) {
    long r;

    // Operator fara operanzi suficienti
    r = evaluatePostfix("+");
    sd_assert("invalid - Test 1 picat", r == LONG_MIN);

    r = evaluatePostfix("3 +");
    sd_assert("invalid - Test 2 picat", r == LONG_MIN);

    // Prea multi operanzi ramasi pe stiva
    r = evaluatePostfix("1 2 3 +");
    sd_assert("invalid - Test 3 picat", r == LONG_MIN);

    // Sir gol
    r = evaluatePostfix("");
    sd_assert("invalid - Test 4 picat", r == LONG_MIN);

    // Caracter invalid
    r = evaluatePostfix("3 4 x");
    sd_assert("invalid - Test 5 picat", r == LONG_MIN);

    // Impartire prin zero
    r = evaluatePostfix("10 0 /");
    sd_assert("invalid - Test 6 picat (impartire prin 0)", r == LONG_MIN);

    printf("invalid \t\t- Toate testele au trecut!");
    return 0;
}

// =============================================================================
static char *all_tests(void) {
    sd_run_test(test_simple,         3.00);
    sd_run_test(test_compound,       3.00);
    sd_run_test(test_multidigit,     3.00);
    sd_run_test(test_single_operand, 2.00);
    sd_run_test(test_invalid,        4.00);
    return 0;
}

int main(void) {
    char *result = all_tests();
    printf("\n");
    if (result != 0)
        printf("ESUAT: %s\n", result);
    printf("Teste rulate: %d / %d\n", tests_run, 5);
    printf("SCOR Postfix: %.2f / 15.00\n\n", total_score);

    return result != 0;
}
