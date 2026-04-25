#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>

/* Lista dublu-inlantuita generica. Tipul Item trebuie definit inainte de include. */

typedef struct ListNode {
    Item elem;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct List {
    ListNode *first;
    ListNode *last;
} List;

int length(List *list);

ListNode *createNode(Item elem) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    node->elem = elem;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

List *createList(void) {
    List *list = (List *)malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    return list;
}

int isEmpty(List *list) {
    if (list == NULL)
        return 1;
    return (list->first == NULL);
}

void insertAt(List *list, Item elem, int pos) {
    if (list == NULL || pos < 0)
        return;

    int len = length(list);
    if (pos > len)
        return;

    ListNode *newNode = createNode(elem);

    if (list->first == NULL) {
        list->first = newNode;
        list->last = newNode;
        return;
    }

    if (pos == 0) {
        newNode->next = list->first;
        list->first->prev = newNode;
        list->first = newNode;
        return;
    }

    if (pos == len) {
        newNode->prev = list->last;
        list->last->next = newNode;
        list->last = newNode;
        return;
    }

    ListNode *curr = list->first;
    for (int i = 0; i < pos; i++)
        curr = curr->next;
    newNode->next = curr;
    newNode->prev = curr->prev;
    curr->prev->next = newNode;
    curr->prev = newNode;
}

int length(List *list) {
    if (list == NULL)
        return 0;
    int count = 0;
    ListNode *curr = list->first;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

List *destroyList(List *list) {
    if (list == NULL)
        return NULL;
    ListNode *curr = list->first;
    while (curr != NULL) {
        ListNode *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
    return NULL;
}

#endif
