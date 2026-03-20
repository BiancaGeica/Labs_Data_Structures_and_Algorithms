#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 256

typedef char Item;
#include "Stack.h"
#include "parantheses.h"

// -----------------------------------------------------------------------------
// Macros (stil lab03)
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
// Test: paranteze simple - un singur tip
// =============================================================================
static char *test_simple() {
	int res;

	// () - balansat
	res = isBalanced("()", 2);
	if (res != 1) printf("\n  isBalanced(\"()\"): asteptat 1, primit %d\n", res);
	sd_assert("simple - Test 1 picat (\"()\")", res == 1);

	// [] - balansat
	res = isBalanced("[]", 2);
	if (res != 1) printf("\n  isBalanced(\"[]\"): asteptat 1, primit %d\n", res);
	sd_assert("simple - Test 2 picat (\"[]\")", res == 1);

	// {} - balansat
	res = isBalanced("{}", 2);
	if (res != 1) printf("\n  isBalanced(\"{}\"): asteptat 1, primit %d\n", res);
	sd_assert("simple - Test 3 picat (\"{}\")", res == 1);

	// (( - nebalansat
	res = isBalanced("((", 2);
	if (res != 0) printf("\n  isBalanced(\"((\"): asteptat 0, primit %d\n", res);
	sd_assert("simple - Test 4 picat (\"((\")", res == 0);

	// )) - nebalansat
	res = isBalanced("))", 2);
	if (res != 0) printf("\n  isBalanced(\"))\"): asteptat 0, primit %d\n", res);
	sd_assert("simple - Test 5 picat (\"))\")", res == 0);

	// (]) - nebalansat (tip gresit)
	res = isBalanced("(]", 2);
	if (res != 0) printf("\n  isBalanced(\"(]\"): asteptat 0, primit %d\n", res);
	sd_assert("simple - Test 6 picat (\"(]\")", res == 0);

	printf("simple \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: paranteze imbricate
// =============================================================================
static char *test_nested() {
	int res;

	// ((())) - balansat
	res = isBalanced("((()))", 6);
	if (res != 1) printf("\n  isBalanced(\"((()))\"): asteptat 1, primit %d\n", res);
	sd_assert("nested - Test 1 picat (\"((()))\")", res == 1);

	// [[[]]] - balansat
	res = isBalanced("[[[]]]", 6);
	if (res != 1) printf("\n  isBalanced(\"[[[]]]\"): asteptat 1, primit %d\n", res);
	sd_assert("nested - Test 2 picat (\"[[[]]]\")", res == 1);

	// {{{}}} - balansat
	res = isBalanced("{{{}}}", 6);
	if (res != 1) printf("\n  isBalanced(\"{{{}}}\"): asteptat 1, primit %d\n", res);
	sd_assert("nested - Test 3 picat (\"{{{}}}\")", res == 1);

	// {([])} - balansat
	res = isBalanced("{([])}", 6);
	if (res != 1) printf("\n  isBalanced(\"{([])}\"): asteptat 1, primit %d\n", res);
	sd_assert("nested - Test 4 picat (\"{([])}\")", res == 1);

	// {([)]} - nebalansat
	res = isBalanced("{([)]}", 6);
	if (res != 0) printf("\n  isBalanced(\"{([)]}\"): asteptat 0, primit %d\n", res);
	sd_assert("nested - Test 5 picat (\"{([)]}\")", res == 0);

	// {(})[() - nebalansat
	res = isBalanced("{(})[()]", 8);
	if (res != 0) printf("\n  isBalanced(\"{(})[()]\"): asteptat 0, primit %d\n", res);
	sd_assert("nested - Test 6 picat (\"{(})[()]\")", res == 0);

	printf("nested \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: paranteze complexe
// =============================================================================
static char *test_complex() {
	int res;
	const char *s;

	// (()( - nebalansat (neincheiat)
	s = "((()(" ;
	res = isBalanced(s, strlen(s));
	if (res != 0) printf("\n  isBalanced(\"%s\"): asteptat 0, primit %d\n", s, res);
	sd_assert("complex - Test 1 picat (\"((()(\")", res == 0);

	// ((()) - nebalansat (lipseste una)
	s = "((())";
	res = isBalanced(s, strlen(s));
	if (res != 0) printf("\n  isBalanced(\"%s\"): asteptat 0, primit %d\n", s, res);
	sd_assert("complex - Test 2 picat (\"((())\")", res == 0);

	// secventa lunga balansata
	s = "((()))()(())(((())))()()" ;
	res = isBalanced(s, strlen(s));
	if (res != 1) printf("\n  isBalanced(\"%s\"): asteptat 1, primit %d\n", s, res);
	sd_assert("complex - Test 3 picat (secventa lunga balansata)", res == 1);

	// secventa lunga nebalansata - prea multe )
	s = "()()(())((()))()((())))";
	res = isBalanced(s, strlen(s));
	if (res != 0) printf("\n  isBalanced(\"%s\"): asteptat 0, primit %d\n", s, res);
	sd_assert("complex - Test 4 picat (secventa lunga nebalansata 1)", res == 0);

	// secventa lunga nebalansata
	s = "()()(())((())))()((()))";
	res = isBalanced(s, strlen(s));
	if (res != 0) printf("\n  isBalanced(\"%s\"): asteptat 0, primit %d\n", s, res);
	sd_assert("complex - Test 5 picat (secventa lunga nebalansata 2)", res == 0);

	// alta secventa lunga nebalansata
	s = "()()(())((())))((())))";
	res = isBalanced(s, strlen(s));
	if (res != 0) printf("\n  isBalanced(\"%s\"): asteptat 0, primit %d\n", s, res);
	sd_assert("complex - Test 6 picat (secventa lunga nebalansata 3)", res == 0);

	// secventa lunga balansata
	s = "((()))()(())(((())))(())";
	res = isBalanced(s, strlen(s));
	if (res != 1) printf("\n  isBalanced(\"%s\"): asteptat 1, primit %d\n", s, res);
	sd_assert("complex - Test 7 picat (secventa lunga balansata 2)", res == 1);

	// secventa foarte lunga balansata
	s = "((()))()(())((((()))))(((((())))))";
	res = isBalanced(s, strlen(s));
	if (res != 1) printf("\n  isBalanced(\"%s\"): asteptat 1, primit %d\n", s, res);
	sd_assert("complex - Test 8 picat (secventa foarte lunga balansata)", res == 1);

	printf("complex \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Main
// =============================================================================
static char *all_tests() {
	sd_run_test(test_simple,  0.50);
	sd_run_test(test_nested,  0.75);
	sd_run_test(test_complex, 0.75);
	return 0;
}

int main(void) {
	char *result = all_tests();

	printf("\n");
	if (result != 0) {
		printf("ESUAT: %s\n", result);
	}
	printf("Teste rulate: %d / %d\n", tests_run, 3);
	printf("SCOR Parantheses: %.2f / 2.00\n\n", total_score);

	return result != 0;
}
