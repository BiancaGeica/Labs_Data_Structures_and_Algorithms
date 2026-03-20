#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

Queue* createQueue(void){
	// TODO: Cerinta 2
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL)
		exit(1);
	
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;

	return queue;
} 

int isQueueEmpty(Queue *q){
	// TODO: Cerinta 2
	return q->front == NULL;
}

void enqueue(Queue *q, Item elem){
	// TODO: Cerinta 2
	QueueNode *new = (QueueNode *)malloc(sizeof(QueueNode));
	if(new == NULL) {
		return;
	}
	new->elem = elem;
	new->next = NULL;

	if (isQueueEmpty(q)) {
		q->rear = new;
		q->front = new;
		q->size++;
		return;
	}

	q->rear->next = new;
	q->rear = new;
	q->size++;
}

Item front(Queue* q){
	// TODO: Cerinta 2
	return q->front->elem;
}

void dequeue(Queue* q){
	// TODO: Cerinta 2
	if (q->front == NULL) {
		return;
	}
	
	QueueNode *temp = q->front;
	q->front = q->front->next;
	free(temp);

	q->size--;

	if (q->front == NULL)
		q->rear = NULL;
}

void destroyQueue(Queue *q){
	// TODO: Cerinta 2
	while (q->front != NULL) {
		dequeue(q);
	}
	free(q);
}

#endif
