#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
    T value;
    struct node* next;
} TNode, *TSortedList;

// TODO : Cerința 1 & Cerința 2

TSortedList create(T value) {
    // TODO (se va completa la Cerința 2)
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node->value = value;
    node->next = NULL;
    return node;
}

int isEmpty(TSortedList list) {
    if (list == NULL)
        return 1;
    return 0;
}

int contains(TSortedList list, T element) {
    TNode* temp = list; //luam o variabila temporara pentru a nu pierde head-ul listei
    while (temp != NULL) { //Null este finalul listei
        if (temp->value == element) { 
            return 1;
        }
        temp = temp->next;
    }

    return 0;
}

TSortedList insert(TSortedList list, T element) {
    TNode* node = create(element);

    if (isEmpty(list)){ //if the list is empty
        node->next = NULL;
        return node;
    }

    if (list->value > element) { //insert at the start
        node->next = list;
        return node;
    }

    TSortedList iter = list;
    while(iter->next != NULL) { //insert inside
        if (iter->next->value > element) {
            node->next = iter->next;
            iter->next = node;
            return list;
        }
        iter = iter->next;
    }

    iter->next = node; //insert at the end of the list
    return list;
}

TSortedList deleteOnce(TSortedList list, T element) {
    if (isEmpty(list))
        return list; //if the list is empty, we have nothing to do

    if (list->value == element) {
        TNode *iter = list;
        list = list->next;
        free(iter);
        return list;
    }
    
    TNode *temp = list;
    while (temp->next != NULL && temp->next->value != element) {
        temp = temp->next;
    }

    if (temp->next != NULL) {
        TNode *wanted = temp->next;
        temp->next = temp->next->next; 
        free(wanted);
    }

    return list;
}

long length(TSortedList list) {
    if(isEmpty(list)){
        return 0;
    }
    long len = 0;
    TNode *temp = list;
    while(temp != NULL) {
        len++;
        temp = temp->next;
    }
    return len;

    /*if(isEmpty(list))
        return 0;
    
    return 1 + length(list->next);*/
}

T getNth(TSortedList list, int n) {
    TNode *temp = list;
    for(size_t i = 1; i < n; i++) {
        if(temp != NULL) {
            temp = temp->next;
        }
    }
    return temp->value;
}

TSortedList freeTSortedList(TSortedList list) {
    //free(list); ELIBEREAZA DOAR PRIMUL NOD
    TNode *temp;
    while (list != NULL) {
        temp = list;       
        list = list->next;
        free(temp);
    }
    return NULL;
}

#endif
