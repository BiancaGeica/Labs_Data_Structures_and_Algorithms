#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode{
	Item elem;
	struct StackNode *next;
} StackNode;

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
} Stack;

Stack* createStack(void){
	// TODO: Cerinta 1
	Stack *stack;
	stack = (Stack*)malloc(sizeof(Stack));
	stack->size = 0;
	stack->head = NULL;
	return stack;
}

int isStackEmpty(Stack* stack){
	// TODO: Cerinta 1
	//if (stack->head == NULL)
	//	return 1;
	//return 0;

	return stack->head == NULL;
}

void push(Stack *stack, Item elem){
	// TODO: Cerinta 1
	StackNode* new = (StackNode*)malloc(sizeof(StackNode));
	new->elem = elem;
	new->next = stack->head;

	stack->size++;
	stack->head = new;
}

Item top(Stack *stack){	
	// TODO: Cerinta 1
	return stack->head->elem;
} 

void pop(Stack *stack){
	// TODO: Cerinta 1
	if (stack->head == NULL){
		return;
	}
	StackNode* temp = stack->head;
	stack->head = stack->head->next;
	free(temp);

	stack->size--;
}

void destroyStack(Stack *stack){
	// TODO: Cerinta 1
	while (stack->head != NULL) {
		pop(stack);
	}
	free(stack);
}

#endif 
