#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct OrderedSet {
    T *elements;
    long size;
    long capacity;
} OrderedSet;

OrderedSet *createOrderedSet(long initialCapacity) {
    OrderedSet *newSet = (OrderedSet *)malloc(sizeof(OrderedSet));
    newSet->size = 0;
    newSet->capacity = initialCapacity;
    newSet->elements = (T *)malloc(initialCapacity * sizeof(T));
    return newSet;
}

int contains(OrderedSet *set, const T element) {
    long left = 0, right = set->size - 1;
    while (left <= right) { /////AICI NU ERA EGALUL
        long mid = (left + right) / 2;
        if (set->elements[mid] == element)
            return 1;
        else if (set->elements[mid] < element)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return 0;
}

void add(OrderedSet *set, const T newElement) {
    if (contains(set, newElement))
        return;

    if (set->size == set->capacity) {
        set->capacity *= 2;
        set->elements = (T *)realloc(set->elements, set->capacity * sizeof(T));
    }

    long i = set->size - 1;
    while (i >= 0 && set->elements[i] > newElement) {
        set->elements[i + 1] = set->elements[i]; //AICI AM STERS UN +1 PUS AIUREA
        i--;
    }
    set->elements[i + 1] = newElement;
    set->size++;
}

void destroyOrderedSet(OrderedSet *set) {
    if (set == NULL)
        return;
    free(set->elements);
    free(set);
}

#endif
