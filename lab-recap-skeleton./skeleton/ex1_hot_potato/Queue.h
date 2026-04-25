#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

/* Coada generica. Tipul Item trebuie sa fie definit inainte de include. */

typedef struct QueueNode {
    Item elem;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
    long size;
} Queue;

Queue *createQueue(void) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int isQueueEmpty(Queue *q) {
    return (q->front == NULL);
}

void enqueue(Queue *q, Item elem) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->elem = elem;
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

Item front(Queue *q) {
    return q->front->elem;
}

void dequeue(Queue *q) {
    if (q->front == NULL)
        return;
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    q->size--;
}

void destroyQueue(Queue *q) {
    if (q == NULL)
        return;
    QueueNode *curr = q->front;
    while (curr != NULL) {
        QueueNode *next = curr->next;
        free(curr);
        curr = next;
    }
    free(q);
}

#endif
