#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>                                          // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

/*
  OrderedSet represents a set with elements of type T stored in memory
  in an ordered fashion.
*/

typedef struct OrderedSet {
  T* elements;                    // pointer to where the elements are in memory
  long size;                                // the number of elements in the set
  long capacity;                      // the current capacity (allocated memory)
} OrderedSet;

// -----------------------------------------------------------------------------

/*
  Function that creates an OrderedSet with elements of type T with a given
  initial capacity.
*/

OrderedSet* createOrderedSet(long initialCapacity) {

  OrderedSet* newSet = (OrderedSet*) malloc(sizeof(OrderedSet));

  newSet->size = 0;                        // initially, the set is empty
  newSet->capacity = initialCapacity;
  newSet->elements = (T*) malloc(initialCapacity * sizeof(T));

  return newSet;
}

// -----------------------------------------------------------------------------

/*
  Function that checks if an element exists in a given set. As the
  elements of the set are ordered in memory, this function uses binary
  search.

  Function returns 1 if the given element exists in the set, and 0
  otherwise.
*/

int contains(OrderedSet* set, const T element) {
    long left = 0;
    long right = set->size - 1;

    while (left <= right) {
        long mid = (left + right) / 2;

        if (*(set->elements + mid) == element) {
            return 1; 
        }

        if (*(set->elements + mid) < element) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------

/*
  Function that adds a new element to a set. If the element was
  already in the set, nothing changes. Otherwise, the element is added
  to the set. In the end, the elements must be in ascending order.
*/

void add(OrderedSet* set, const T newElement) {
    if (contains(set, newElement)){ //BEFORE: mississippi: There are 11 letters: i i i i m p p s s s s
        return;
    }

    if (set->size == set->capacity){
        set->capacity += 1;
        set->elements = (T*) realloc(set->elements, set->capacity * sizeof(T));
    }

    int i;
    for (i = set->size - 1; i >= 0 && set->elements[i] > newElement; i--) {
        set->elements[i + 1] = set->elements[i];
    }

    set->elements[i + 1] = newElement;
    set->size++;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the union of
  the two
*/

OrderedSet* unionOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  OrderedSet* s3 = createOrderedSet(s1->size + s2->size);
  
  long i = 0; 
  long j = 0; 
  long k = 0;

  while (i < s1->size && j < s2->size) {
    if (s1->elements[i] < s2->elements[j]) {
      s3->elements[k] = s1->elements[i];
      i++;
      k++;
    } 
    else if (s1->elements[i] > s2->elements[j]) {
      s3->elements[k] = s2->elements[j];
      j++;
      k++;
    } 
    else {
      s3->elements[k] = s1->elements[i];
      i++;
      j++;
      k++;
    }
  }

  while (i < s1->size) {
    s3->elements[k] = s1->elements[i];
    i++;
    k++;
  }

  while (j < s2->size) {
    s3->elements[k] = s2->elements[j];
    j++;
    k++;
  }

  s3->size = k;

  return s3;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the
  intersection of the two
*/

OrderedSet* intersectOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  OrderedSet* s3 = createOrderedSet(s1->size + s2->size);
  
  long i = 0; 
  long j = 0; 
  long k = 0;

  while (i < s1->size && j < s2->size) {
    if (s1->elements[i] < s2->elements[j]) {
      i++;
    } 
    else if (s1->elements[i] > s2->elements[j]) {
      j++;
    } 
    else {
      s3->elements[k] = s1->elements[i];
      i++;
      j++;
      k++;
    }
  }

  s3->size = k;

  return s3;
}


// -----------------------------------------------------------------------------

#endif
