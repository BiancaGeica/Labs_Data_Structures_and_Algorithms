#include <stdio.h>
#include <string.h>

typedef long T;
#include "OrderedSet.h"

/*
  Function that returns an ordered set with long elements between
  @start and @end that are multiples of @div
*/

OrderedSet* multiples(long start, long end, long nr) {
    OrderedSet* set = createOrderedSet(1);    

    for (long i = start; i <= end; i++) {
        if (i % nr == 0) {
            add(set, i);
        }
    }
    return set;
}

// -----------------------------------------------------------------------------

/*
  Function that prints the size and the elements of an ordered set.
*/

void printOrderedLongSet(OrderedSet* set) {
    printf("There are %ld elements:", set->size);
    for (long i = 0; i < set->size; i++) {
        printf(" %ld", set->elements[i]);
    }
    printf("\n");
}

// -----------------------------------------------------------------------------


int main() {
    OrderedSet* s1 = multiples(4, 25, 3);
    printOrderedLongSet(s1);

    OrderedSet* s2 = multiples(5, 30, 4);
    printOrderedLongSet(s2);

    OrderedSet* sU = unionOrderedSets(s1, s2);
    printOrderedLongSet(sU);

    OrderedSet* sI = intersectOrderedSets(s1, s2);
    printOrderedLongSet(sI);

    return 0;
}
