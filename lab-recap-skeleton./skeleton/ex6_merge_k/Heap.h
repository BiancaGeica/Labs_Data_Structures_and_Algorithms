#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>

/* Max-heap (Lab 08). Se presupune ca Item este deja definit. */

typedef struct {
    int prior;
    Item data;
} ItemType;

typedef struct heap {
    long maxHeapSize;
    long size;
    ItemType *elem;
} PriQueue, *APriQueue;

PriQueue *makeQueue(int maxHeapSize) {
    APriQueue pq = (APriQueue)malloc(sizeof(PriQueue));
    pq->maxHeapSize = maxHeapSize;
    pq->size = 0;
    pq->elem = (ItemType *)malloc(sizeof(ItemType) * maxHeapSize);
    return pq;
}

int getLeftChild(int i)  { return 2 * i + 1; }
int getRightChild(int i) { return 2 * i + 2; }
int getParent(int i)     { return (i - 1) / 2; }

ItemType getMax(APriQueue h) {
    return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
    while (idx > 0) {
        int p = getParent(idx);
        if (h->elem[p].prior < h->elem[idx].prior) {
            ItemType t = h->elem[p];
            h->elem[p] = h->elem[idx];
            h->elem[idx] = t;
            idx = p;
        } else {
            break;
        }
    }
}

void heapInsert(PriQueue *h, ItemType x) {
    if (h->size == h->maxHeapSize) {
        h->maxHeapSize *= 2;
        h->elem = (ItemType *)realloc(h->elem, sizeof(ItemType) * h->maxHeapSize);
    }
    h->elem[h->size] = x;
    h->size++;
    siftUp(h, h->size - 1);
}

void siftDown(APriQueue h, int idx) {
    while (1) {
        int l = getLeftChild(idx);
        int r = getRightChild(idx);
        int max = idx;
        if (l < h->size && h->elem[l].prior > h->elem[max].prior) max = l;
        if (r < h->size && h->elem[r].prior > h->elem[max].prior) max = r;
        if (max == idx) break;
        ItemType t = h->elem[idx];
        h->elem[idx] = h->elem[max];
        h->elem[max] = t;
        idx = max;
    }
}

void removeMax(APriQueue h) {
    if (h->size == 0) return;
    h->elem[0] = h->elem[h->size - 1];
    h->size--;
    if (h->size > 0) siftDown(h, 0);
}

void freeQueue(APriQueue h) {
    free(h->elem);
    free(h);
}

#endif
