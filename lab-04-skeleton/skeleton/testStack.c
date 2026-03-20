#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  Generic Item type definition
 */
typedef int Item;

/**
 * The Stack interface
 */
#include "Stack.h"

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

// -----------------------------------------------------------------------------
// Functii auxiliare pentru teste
// -----------------------------------------------------------------------------

// Construieste manual o stiva cu elementele din array (primul element = baza)
Stack* manualBuild(int *elems, int n) {
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->head = NULL;
	stack->size = 0;
	for (int i = n - 1; i >= 0; i--) {
		StackNode *node = (StackNode *)malloc(sizeof(StackNode));
		node->elem = elems[i];
		node->next = stack->head;
		stack->head = node;
		stack->size++;
	}
	return stack;
}

// Elibereaza memoria unei stive
void manualFree(Stack *stack) {
	if (stack == NULL) return;
	StackNode *curr = stack->head;
	while (curr != NULL) {
		StackNode *next = curr->next;
		free(curr);
		curr = next;
	}
	free(stack);
}

// Numara nodurile unei stive
int manualLength(Stack *stack) {
	int count = 0;
	StackNode *curr = stack->head;
	while (curr != NULL) {
		count++;
		curr = curr->next;
	}
	return count;
}

// Verifica daca stiva contine elementele asteptate (top -> ... -> baza)
int manualCheck(Stack *stack, int *expected, int n) {
	StackNode *curr = stack->head;
	for (int i = 0; i < n; i++) {
		if (curr == NULL) return 0;
		if (curr->elem != expected[i]) return 0;
		curr = curr->next;
	}
	return (curr == NULL);
}

void printStack(Stack *stack) {
	if (stack == NULL || stack->head == NULL) {
		printf("[]");
		return;
	}
	StackNode *it = stack->head;
	printf("[");
	while (it != NULL) {
		printf("%d", it->elem);
		if (it->next != NULL) printf(", ");
		it = it->next;
	}
	printf("]");
}

// =============================================================================
// Test: createStack
// =============================================================================
static char *test_create() {
	Stack *stack = createStack();
	if (stack == NULL) {
		printf("\n  createStack() a returnat NULL\n");
		return "create - Test 1 picat (createStack returneaza NULL)";
	}
	if (stack->head != NULL)
		printf("\n  createStack()->head != NULL (asteptat: NULL)\n");
	sd_assert("create - Test 2 picat (head != NULL)", stack->head == NULL);
	if (stack->size != 0)
		printf("\n  createStack()->size: asteptat 0, primit %ld\n", stack->size);
	sd_assert("create - Test 3 picat (size != 0)", stack->size == 0);

	free(stack);

	// Al doilea createStack
	Stack *stack2 = createStack();
	sd_assert("create - Test 4 picat (al doilea createStack returneaza NULL)", stack2 != NULL);
	sd_assert("create - Test 5 picat (al doilea createStack, head != NULL)", stack2->head == NULL);
	sd_assert("create - Test 6 picat (al doilea createStack, size != 0)", stack2->size == 0);
	free(stack2);

	printf("create \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: isStackEmpty
// =============================================================================
static char *test_isEmpty() {
	int res;

	// Stiva goala
	Stack *stack = createStack();
	res = isStackEmpty(stack);
	if (res != 1)
		printf("\n  isStackEmpty(stiva goala): asteptat 1, primit %d\n", res);
	sd_assert("isStackEmpty - Test 1 picat (stiva goala)", res == 1);

	// Stiva cu 1 nod (construit manual)
	StackNode aNode = {2, NULL};
	stack->head = &aNode;
	stack->size = 1;
	res = isStackEmpty(stack);
	if (res != 0)
		printf("\n  isStackEmpty(stiva cu 1 nod): asteptat 0, primit %d\n", res);
	sd_assert("isStackEmpty - Test 2 picat (stiva cu 1 nod)", res == 0);

	// Golim manual
	stack->head = NULL;
	stack->size = 0;
	res = isStackEmpty(stack);
	if (res != 1)
		printf("\n  isStackEmpty(dupa golire manuala): asteptat 1, primit %d\n", res);
	sd_assert("isStackEmpty - Test 3 picat (dupa golire manuala)", res == 1);

	sd_assert("isStackEmpty - Test 4 picat (size != 0)", stack->size == 0);

	free(stack);

	printf("isStackEmpty \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: push
// =============================================================================
static char *test_push() {
	Stack *stack = createStack();

	// Push pe stiva goala
	push(stack, 1);
	if (stack->head == NULL) {
		printf("\n  push([], 1): head e NULL dupa push\n");
		free(stack);
		return "push - Test 1 picat (head NULL dupa push)";
	}
	if (stack->head->elem != 1)
		printf("\n  push([], 1): head->elem asteptat 1, primit %d\n", stack->head->elem);
	sd_assert("push - Test 2 picat (elem != 1)", stack->head->elem == 1);
	if (stack->size != 1)
		printf("\n  push([], 1): size asteptat 1, primit %ld\n", stack->size);
	sd_assert("push - Test 3 picat (size != 1)", stack->size == 1);
	sd_assert("push - Test 4 picat (head->next != NULL)", stack->head->next == NULL);

	// Push al doilea element
	push(stack, 2);
	sd_assert("push - Test 5 picat (head NULL dupa al doilea push)", stack->head != NULL);
	if (stack->head->elem != 2)
		printf("\n  push([1], 2): head->elem asteptat 2, primit %d\n", stack->head->elem);
	sd_assert("push - Test 6 picat (head->elem != 2)", stack->head->elem == 2);
	if (stack->size != 2)
		printf("\n  push([1], 2): size asteptat 2, primit %ld\n", stack->size);
	sd_assert("push - Test 7 picat (size != 2)", stack->size == 2);
	sd_assert("push - Test 8 picat (head->next == NULL)", stack->head->next != NULL);
	sd_assert("push - Test 9 picat (head->next->elem != 1)", stack->head->next->elem == 1);
	sd_assert("push - Test 10 picat (head->next->next != NULL)", stack->head->next->next == NULL);

	// Push mai multe
	push(stack, 3);
	push(stack, 4);
	push(stack, 5);
	if (stack->size != 5)
		printf("\n  dupa 5 push-uri: size asteptat 5, primit %ld\n", stack->size);
	sd_assert("push - Test 11 picat (size != 5)", stack->size == 5);
	if (stack->head->elem != 5)
		printf("\n  dupa push(5): head->elem asteptat 5, primit %d\n", stack->head->elem);
	sd_assert("push - Test 12 picat (head->elem != 5)", stack->head->elem == 5);
	sd_assert("push - Test 13 picat (head->next->next->elem != 3)",
		stack->head->next->next->elem == 3);
	sd_assert("push - Test 14 picat (head->next->next->next->elem != 2)",
		stack->head->next->next->next->elem == 2);

	manualFree(stack);

	printf("push \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: top
// =============================================================================
static char *test_top() {
	int res;

	// Stiva cu 1 element
	int elems1[] = {5};
	Stack *stack = manualBuild(elems1, 1);
	res = top(stack);
	if (res != 5)
		printf("\n  top([5]): asteptat 5, primit %d\n", res);
	sd_assert("top - Test 1 picat (top pe stiva cu 1 elem)", res == 5);
	// top nu trebuie sa scoata elementul
	sd_assert("top - Test 2 picat (top a modificat size)", stack->size == 1);
	sd_assert("top - Test 3 picat (top a modificat head)", stack->head != NULL);
	manualFree(stack);

	// Stiva cu mai multe elemente: top = 8, 7, 6, 5, 4, 3, 2, 1
	int elems2[] = {8, 7, 6, 5, 4, 3, 2, 1};
	stack = manualBuild(elems2, 8);
	res = top(stack);
	if (res != 8)
		printf("\n  top([8,7,6,5,4,3,2,1]): asteptat 8, primit %d\n", res);
	sd_assert("top - Test 4 picat (top pe stiva mare)", res == 8);
	sd_assert("top - Test 5 picat (top a modificat size)", stack->size == 8);

	// Apel repetat top - trebuie sa returneze acelasi element
	int res2 = top(stack);
	sd_assert("top - Test 6 picat (al doilea apel top da alt rezultat)", res2 == 8);
	sd_assert("top - Test 7 picat (size s-a schimbat dupa al doilea top)", stack->size == 8);

	manualFree(stack);

	// Stiva cu un singur element diferit
	int elems3[] = {42};
	stack = manualBuild(elems3, 1);
	res = top(stack);
	sd_assert("top - Test 8 picat (top([42]) != 42)", res == 42);
	manualFree(stack);

	printf("top \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: pop
// =============================================================================
static char *test_pop() {
	// Stiva cu 5 elemente: top = 5, 4, 3, 2, 1
	int elems[] = {5, 4, 3, 2, 1};
	Stack *stack = manualBuild(elems, 5);

	pop(stack);
	if (stack->size != 4)
		printf("\n  pop([5,4,3,2,1]): size asteptat 4, primit %ld\n", stack->size);
	sd_assert("pop - Test 1 picat (size != 4 dupa 1 pop)", stack->size == 4);
	if (stack->head == NULL) {
		printf("\n  pop: head e NULL dupa un pop pe stiva cu 5 elemente\n");
		free(stack);
		return "pop - Test 2 picat (head NULL)";
	}
	if (stack->head->elem != 4)
		printf("\n  pop: head->elem asteptat 4, primit %d\n", stack->head->elem);
	sd_assert("pop - Test 3 picat (head->elem != 4)", stack->head->elem == 4);

	pop(stack);
	pop(stack);
	sd_assert("pop - Test 4 picat (size != 2 dupa 3 pop-uri)", stack->size == 2);
	sd_assert("pop - Test 5 picat (head->elem != 2)", stack->head->elem == 2);

	pop(stack);
	sd_assert("pop - Test 6 picat (size != 1)", stack->size == 1);
	sd_assert("pop - Test 7 picat (head->elem != 1)", stack->head->elem == 1);
	sd_assert("pop - Test 8 picat (head->next != NULL)", stack->head->next == NULL);

	pop(stack);
	sd_assert("pop - Test 9 picat (size != 0 dupa pop total)", stack->size == 0);
	if (stack->head != NULL)
		printf("\n  pop: head trebuie sa fie NULL dupa pop total\n");
	sd_assert("pop - Test 10 picat (head != NULL dupa pop total)", stack->head == NULL);

	free(stack);

	// Pop + push combinat
	int elems2[] = {3, 2, 1};
	stack = manualBuild(elems2, 3);
	push(stack, 4);
	pop(stack);
	sd_assert("pop - Test 11 picat (push+pop: size != 3)", stack->size == 3);
	sd_assert("pop - Test 12 picat (push+pop: head->elem != 3)", stack->head->elem == 3);

	pop(stack);
	pop(stack);
	sd_assert("pop - Test 13 picat (head->elem != 1)", stack->head->elem == 1);
	sd_assert("pop - Test 14 picat (head->next != NULL)", stack->head->next == NULL);

	manualFree(stack);

	printf("pop \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: destroyStack
// =============================================================================
static char *test_destroyStack() {
	// Stiva goala
	Stack *stack = createStack();
	destroyStack(stack);

	// 1 element
	stack = createStack();
	push(stack, 1);
	destroyStack(stack);

	// 5 elemente
	stack = createStack();
	for (int i = 0; i < 5; i++)
		push(stack, i);
	destroyStack(stack);

	// 10 elemente
	stack = createStack();
	for (int i = 0; i < 10; i++)
		push(stack, i);
	destroyStack(stack);

	printf("destroyStack \t\t- Toate testele au trecut!\n");
	printf("  *Destroy se va verifica cu valgrind*");
	return 0;
}

// =============================================================================
// Main
// =============================================================================
static char *all_tests() {
	sd_run_test(test_create,       0.30);
	sd_run_test(test_isEmpty,      0.20);
	sd_run_test(test_push,         1.00);
	sd_run_test(test_top,          0.50);
	sd_run_test(test_pop,          1.00);
	sd_run_test(test_destroyStack, 1.00);
	return 0;
}

int main(void) {
	char *result = all_tests();

	printf("\n");
	if (result != 0) {
		printf("ESUAT: %s\n", result);
	}
	printf("Teste rulate: %d / %d\n", tests_run, 6);
	printf("SCOR Stack: %.2f / 4.00\n\n", total_score);

	return result != 0;
}
