#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  Generic Item type definition
 */
typedef int Item;

/**
 * The Queue interface
 */
#include "Queue.h"

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

// Construieste manual o coada cu elementele din array (primul = front)
Queue* manualBuild(int *elems, int n) {
	Queue *q = (Queue *)malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	for (int i = 0; i < n; i++) {
		QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
		node->elem = elems[i];
		node->next = NULL;
		if (q->rear == NULL) {
			q->front = node;
			q->rear = node;
		} else {
			q->rear->next = node;
			q->rear = node;
		}
		q->size++;
	}
	return q;
}

// Elibereaza memoria unei cozi
void manualFree(Queue *q) {
	if (q == NULL) return;
	QueueNode *curr = q->front;
	while (curr != NULL) {
		QueueNode *next = curr->next;
		free(curr);
		curr = next;
	}
	free(q);
}

// Numara nodurile unei cozi
int manualLength(Queue *q) {
	int count = 0;
	QueueNode *curr = q->front;
	while (curr != NULL) {
		count++;
		curr = curr->next;
	}
	return count;
}

// Verifica daca coada contine elementele asteptate (front -> ... -> rear)
int manualCheck(Queue *q, int *expected, int n) {
	QueueNode *curr = q->front;
	for (int i = 0; i < n; i++) {
		if (curr == NULL) return 0;
		if (curr->elem != expected[i]) return 0;
		curr = curr->next;
	}
	return (curr == NULL);
}

void printQueue(Queue *q) {
	if (q == NULL || q->front == NULL) {
		printf("[]");
		return;
	}
	QueueNode *it = q->front;
	printf("[");
	while (it != NULL) {
		printf("%d", it->elem);
		if (it->next != NULL) printf(", ");
		it = it->next;
	}
	printf("]");
}

// =============================================================================
// Test: createQueue
// =============================================================================
static char *test_create() {
	Queue *q = createQueue();
	if (q == NULL) {
		printf("\n  createQueue() a returnat NULL\n");
		return "create - Test 1 picat (createQueue returneaza NULL)";
	}
	if (q->front != NULL)
		printf("\n  createQueue()->front != NULL (asteptat: NULL)\n");
	sd_assert("create - Test 2 picat (front != NULL)", q->front == NULL);
	if (q->rear != NULL)
		printf("\n  createQueue()->rear != NULL (asteptat: NULL)\n");
	sd_assert("create - Test 3 picat (rear != NULL)", q->rear == NULL);
	if (q->size != 0)
		printf("\n  createQueue()->size: asteptat 0, primit %ld\n", q->size);
	sd_assert("create - Test 4 picat (size != 0)", q->size == 0);

	free(q);

	// Al doilea createQueue
	Queue *q2 = createQueue();
	sd_assert("create - Test 5 picat (al doilea createQueue returneaza NULL)", q2 != NULL);
	sd_assert("create - Test 6 picat (al doilea createQueue, front != NULL)", q2->front == NULL);
	free(q2);

	printf("create \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: isQueueEmpty
// =============================================================================
static char *test_isEmpty() {
	int res;

	// Coada goala
	Queue *q = createQueue();
	res = isQueueEmpty(q);
	if (res != 1)
		printf("\n  isQueueEmpty(coada goala): asteptat 1, primit %d\n", res);
	sd_assert("isQueueEmpty - Test 1 picat (coada goala)", res == 1);

	// Coada cu 1 nod (construit manual)
	QueueNode aNode = {2, NULL};
	q->front = &aNode;
	q->rear = &aNode;
	q->size = 1;
	res = isQueueEmpty(q);
	if (res != 0)
		printf("\n  isQueueEmpty(coada cu 1 nod): asteptat 0, primit %d\n", res);
	sd_assert("isQueueEmpty - Test 2 picat (coada cu 1 nod)", res == 0);

	// Golim manual
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	res = isQueueEmpty(q);
	if (res != 1)
		printf("\n  isQueueEmpty(dupa golire manuala): asteptat 1, primit %d\n", res);
	sd_assert("isQueueEmpty - Test 3 picat (dupa golire manuala)", res == 1);

	sd_assert("isQueueEmpty - Test 4 picat (size != 0)", q->size == 0);

	free(q);

	printf("isQueueEmpty \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: enqueue
// =============================================================================
static char *test_enqueue() {
	Queue *q = createQueue();

	// Enqueue pe coada goala
	enqueue(q, 1);
	if (q->front == NULL) {
		printf("\n  enqueue([], 1): front e NULL dupa enqueue\n");
		free(q);
		return "enqueue - Test 1 picat (front NULL dupa enqueue)";
	}
	if (q->rear == NULL) {
		printf("\n  enqueue([], 1): rear e NULL dupa enqueue\n");
		return "enqueue - Test 2 picat (rear NULL dupa enqueue)";
	}
	sd_assert("enqueue - Test 3 picat (front->elem != 1)", q->front->elem == 1);
	sd_assert("enqueue - Test 4 picat (rear->elem != 1)", q->rear->elem == 1);
	sd_assert("enqueue - Test 5 picat (front != rear pe 1 elem)", q->front == q->rear);
	if (q->size != 1)
		printf("\n  enqueue([], 1): size asteptat 1, primit %ld\n", q->size);
	sd_assert("enqueue - Test 6 picat (size != 1)", q->size == 1);
	sd_assert("enqueue - Test 7 picat (front->next != NULL)", q->front->next == NULL);

	// Enqueue al doilea element
	enqueue(q, 2);
	sd_assert("enqueue - Test 8 picat (front NULL dupa al doilea enqueue)", q->front != NULL);
	sd_assert("enqueue - Test 9 picat (rear NULL dupa al doilea enqueue)", q->rear != NULL);
	sd_assert("enqueue - Test 10 picat (front == rear pe 2 elemente)", q->front != q->rear);
	if (q->size != 2)
		printf("\n  enqueue(2): size asteptat 2, primit %ld\n", q->size);
	sd_assert("enqueue - Test 11 picat (size != 2)", q->size == 2);
	if (q->front->elem != 1)
		printf("\n  dupa enqueue(2): front->elem asteptat 1, primit %d\n", q->front->elem);
	sd_assert("enqueue - Test 12 picat (front->elem != 1)", q->front->elem == 1);
	if (q->rear->elem != 2)
		printf("\n  dupa enqueue(2): rear->elem asteptat 2, primit %d\n", q->rear->elem);
	sd_assert("enqueue - Test 13 picat (rear->elem != 2)", q->rear->elem == 2);
	sd_assert("enqueue - Test 14 picat (rear->next != NULL)", q->rear->next == NULL);
	sd_assert("enqueue - Test 15 picat (front->next->next != NULL)", q->front->next->next == NULL);

	// Enqueue mai multe
	enqueue(q, 3);
	enqueue(q, 4);
	enqueue(q, 5);
	if (q->size != 5)
		printf("\n  dupa 5 enqueue-uri: size asteptat 5, primit %ld\n", q->size);
	sd_assert("enqueue - Test 16 picat (size != 5)", q->size == 5);
	sd_assert("enqueue - Test 17 picat (rear->elem != 5)", q->rear->elem == 5);
	sd_assert("enqueue - Test 18 picat (front->elem != 1)", q->front->elem == 1);
	sd_assert("enqueue - Test 19 picat (front->next->next->elem != 3)",
		q->front->next->next->elem == 3);
	sd_assert("enqueue - Test 20 picat (front->next->next->next->elem != 4)",
		q->front->next->next->next->elem == 4);

	manualFree(q);

	printf("enqueue \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: front
// =============================================================================
static char *test_front() {
	int res;

	// Coada cu 1 element
	int elems1[] = {5};
	Queue *q = manualBuild(elems1, 1);
	res = front(q);
	if (res != 5)
		printf("\n  front([5]): asteptat 5, primit %d\n", res);
	sd_assert("front - Test 1 picat (front pe coada cu 1 elem)", res == 5);
	// front nu trebuie sa scoata elementul
	sd_assert("front - Test 2 picat (front a modificat size)", q->size == 1);
	sd_assert("front - Test 3 picat (front a scos elementul)", q->front != NULL);
	manualFree(q);

	// Coada cu mai multe elemente: 1, 2, 3, 4, 5
	int elems2[] = {1, 2, 3, 4, 5};
	q = manualBuild(elems2, 5);
	res = front(q);
	if (res != 1)
		printf("\n  front([1,2,3,4,5]): asteptat 1, primit %d\n", res);
	sd_assert("front - Test 4 picat (front pe coada [1..5])", res == 1);
	sd_assert("front - Test 5 picat (front a modificat size)", q->size == 5);

	// Apel repetat front - trebuie sa returneze acelasi element
	int res2 = front(q);
	sd_assert("front - Test 6 picat (al doilea apel front da alt rezultat)", res2 == 1);
	sd_assert("front - Test 7 picat (size s-a schimbat dupa al doilea front)", q->size == 5);

	manualFree(q);

	// Coada cu un singur element diferit
	int elems3[] = {42};
	q = manualBuild(elems3, 1);
	res = front(q);
	sd_assert("front - Test 8 picat (front([42]) != 42)", res == 42);
	manualFree(q);

	printf("front \t\t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: dequeue
// =============================================================================
static char *test_dequeue() {
	// Coada cu 5 elemente: 1, 2, 3, 4, 5
	int elems[] = {1, 2, 3, 4, 5};
	Queue *q = manualBuild(elems, 5);

	dequeue(q);
	if (q->size != 4)
		printf("\n  dequeue([1,2,3,4,5]): size asteptat 4, primit %ld\n", q->size);
	sd_assert("dequeue - Test 1 picat (size != 4 dupa 1 dequeue)", q->size == 4);
	if (q->front == NULL) {
		printf("\n  dequeue: front e NULL dupa un dequeue pe coada cu 5 elemente\n");
		free(q);
		return "dequeue - Test 2 picat (front NULL)";
	}
	if (q->front->elem != 2)
		printf("\n  dequeue: front->elem asteptat 2, primit %d\n", q->front->elem);
	sd_assert("dequeue - Test 3 picat (front->elem != 2)", q->front->elem == 2);
	sd_assert("dequeue - Test 4 picat (rear->elem != 5)", q->rear->elem == 5);

	dequeue(q);
	dequeue(q);
	sd_assert("dequeue - Test 5 picat (size != 2 dupa 3 dequeue-uri)", q->size == 2);
	sd_assert("dequeue - Test 6 picat (front->elem != 4)", q->front->elem == 4);
	sd_assert("dequeue - Test 7 picat (rear->elem != 5)", q->rear->elem == 5);

	dequeue(q);
	sd_assert("dequeue - Test 8 picat (size != 1)", q->size == 1);
	sd_assert("dequeue - Test 9 picat (front->elem != 5)", q->front->elem == 5);
	sd_assert("dequeue - Test 10 picat (front != rear pe 1 elem)", q->front == q->rear);
	sd_assert("dequeue - Test 11 picat (front->next != NULL)", q->front->next == NULL);

	dequeue(q);
	sd_assert("dequeue - Test 12 picat (size != 0 dupa dequeue total)", q->size == 0);
	if (q->front != NULL)
		printf("\n  dequeue: front trebuie sa fie NULL dupa dequeue total\n");
	sd_assert("dequeue - Test 13 picat (front != NULL dupa dequeue total)", q->front == NULL);
	if (q->rear != NULL)
		printf("\n  dequeue: rear trebuie sa fie NULL dupa dequeue total\n");
	sd_assert("dequeue - Test 14 picat (rear != NULL dupa dequeue total)", q->rear == NULL);

	free(q);

	// Dequeue + enqueue combinat
	int elems2[] = {1, 2, 3};
	q = manualBuild(elems2, 3);
	enqueue(q, 4);
	dequeue(q);
	sd_assert("dequeue - Test 15 picat (enqueue+dequeue: size != 3)", q->size == 3);
	sd_assert("dequeue - Test 16 picat (enqueue+dequeue: front->elem != 2)", q->front->elem == 2);
	sd_assert("dequeue - Test 17 picat (enqueue+dequeue: rear->elem != 4)", q->rear->elem == 4);

	dequeue(q);
	dequeue(q);
	sd_assert("dequeue - Test 18 picat (front->elem != 4)", q->front->elem == 4);
	sd_assert("dequeue - Test 19 picat (rear->next != NULL)", q->rear->next == NULL);

	manualFree(q);

	printf("dequeue \t\t- Toate testele au trecut!");
	return 0;
}

// =============================================================================
// Test: destroyQueue
// =============================================================================
static char *test_destroyQueue() {
	// Coada goala
	Queue *q = createQueue();
	destroyQueue(q);

	// 1 element
	q = createQueue();
	enqueue(q, 1);
	destroyQueue(q);

	// 5 elemente
	q = createQueue();
	for (int i = 0; i < 5; i++)
		enqueue(q, i);
	destroyQueue(q);

	// 10 elemente
	q = createQueue();
	for (int i = 0; i < 10; i++)
		enqueue(q, i);
	destroyQueue(q);

	printf("destroyQueue \t\t- Toate testele au trecut!\n");
	printf("  *Destroy se va verifica cu valgrind*");
	return 0;
}

// =============================================================================
// Main
// =============================================================================
static char *all_tests() {
	sd_run_test(test_create,       0.30);
	sd_run_test(test_isEmpty,      0.20);
	sd_run_test(test_enqueue,      1.00);
	sd_run_test(test_front,        0.50);
	sd_run_test(test_dequeue,      1.00);
	sd_run_test(test_destroyQueue, 1.00);
	return 0;
}

int main(void) {
	char *result = all_tests();

	printf("\n");
	if (result != 0) {
		printf("ESUAT: %s\n", result);
	}
	printf("Teste rulate: %d / %d\n", tests_run, 6);
	printf("SCOR Queue: %.2f / 4.00\n\n", total_score);

	return result != 0;
}
