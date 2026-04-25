#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

/* Stiva generica. Tipul Item trebuie definit inainte de include. */

typedef struct StackNode {
    Item elem;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *head;
    long size;
} Stack;

Stack *createStack(void) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->head = NULL;
    stack->size = 0;
    return stack;
}

int isStackEmpty(Stack *stack) {
    return (stack->head == NULL);
}

void push(Stack *stack, Item elem) {
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    node->elem = elem;
    node->next = stack->head;
    stack->head = node;
    stack->size++;
}

Item top(Stack *stack) {
    return stack->head->elem;
}

void pop(Stack *stack) {
    if (stack->head == NULL)
        return;
    StackNode *temp = stack->head;
    stack->head = stack->head->next;
    free(temp);
    stack->size--;
}

void destroyStack(Stack *stack) {
    if (stack == NULL)
        return;
    StackNode *curr = stack->head;
    while (curr != NULL) {
        StackNode *next = curr->next;
        free(curr);
        curr = next;
    }
    free(stack);
}

#endif
