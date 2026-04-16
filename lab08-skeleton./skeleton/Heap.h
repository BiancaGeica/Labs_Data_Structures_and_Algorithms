#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	//TODO:
	PriQueue *queue = (PriQueue *)malloc(sizeof(PriQueue));
	if (queue == NULL) {
		exit(EXIT_FAILURE);
	}

	queue->size = 0;
	queue->maxHeapSize = maxHeapSize;
	queue->elem = (ItemType *)malloc(queue->maxHeapSize * sizeof(ItemType));
	if (queue->elem == NULL) {
		exit(EXIT_FAILURE);
	}

	return queue;
}

int getLeftChild(int i){
	//TODO:
	return 2 * i + 1;
}

int getRightChild(int i) {
	//TODO:
	return 2 * i + 2;
}

int getParent(int i) {
	//TODO:
	return (i - 1) / 2;
}

ItemType getMax(APriQueue h) {
	//TODO:
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	//TODO:
	while (idx >= 0 && (h->elem[(idx - 1) / 2].prior < h->elem[idx].prior)) {
		ItemType aux = h->elem[(idx - 1) / 2];
		h->elem[(idx - 1) / 2] = h->elem[idx];
		h->elem[idx] = aux;

		idx = (idx - 1) / 2;
	}
}


void insert(PriQueue *h, ItemType x) {
	//TODO:
	if (h->size == h->maxHeapSize) {
		h->maxHeapSize *= 2;
		h->elem = realloc(h->elem, h->maxHeapSize * sizeof(ItemType));
	}

	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx){
	//TODO:
	int maxIndex = idx;
	int l = idx * 2 + 1;

	if(l < h->size && (h->elem[l].prior > h->elem[maxIndex].prior)) {
		maxIndex = l;
	}

	int r = idx * 2 + 2;
	if (r < h->size && (h->elem[r].prior > h->elem[maxIndex].prior)) {
		maxIndex = r;
	}

	if (idx != maxIndex) {
		ItemType aux = h->elem[idx];
		h->elem[idx] = h->elem[maxIndex];
		h->elem[maxIndex] = aux;

		siftDown(h, maxIndex);
	}
}

void removeMax(APriQueue h) {
	//TODO:
	if (h == NULL) {
		return;
	}

	h->elem[0] = h->elem[h->size - 1];
	h->size--;

	siftDown(h, 0);
}

void freeQueue(APriQueue h){
	// TODO:
	if (h != NULL) {
		free(h->elem);
	}
	free(h);
}

#endif

