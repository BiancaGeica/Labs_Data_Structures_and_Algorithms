#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>

/* Lista simplu-inlantuita sortata (Lab 02). T trebuie definit inainte de include. */

typedef struct slnode {
    T value;
    struct slnode *next;
} SLNode, *SortedList;

SortedList slCreate(T value) {
    SLNode *node = (SLNode *)malloc(sizeof(SLNode));
    node->value = value;
    node->next = NULL;
    return node;
}

int slIsEmpty(SortedList list) {
    return list == NULL;
}

/* Insertie sortata: intoarce (eventual) un cap nou de lista. */
SortedList slInsert(SortedList list, T value) {
    SortedList newNode = slCreate(value);
    if (slIsEmpty(list) || list->value >= value) {
        newNode->next = list;
        return newNode;
    }
    SortedList curr = list;
    while (curr->next != NULL && curr->next->value < value)
        curr = curr->next;
    newNode->next = curr->next;
    curr->next = newNode;
    return list;
}

long slLength(SortedList list) {
    long len = 0;
    while (list != NULL) {
        len++;
        list = list->next;
    }
    return len;
}

SortedList slFree(SortedList list) {
    while (list != NULL) {
        SortedList tmp = list->next;
        free(list);
        list = tmp;
    }
    return NULL;
}

#endif
