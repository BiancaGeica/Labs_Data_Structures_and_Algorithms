#include <stdio.h>

typedef int T;
#include "SortedList.h"

TSortedList getNaturals(int A, int B) {
    TSortedList nats = NULL;
    int i;
    for (i = A; i <= B; i++) {
        nats = insert(nats, i);
    }
    return nats;
}

TSortedList getPrimes(int N) {
    TSortedList nats = getNaturals(2, N);
    TSortedList primes = NULL;

    while (!isEmpty(nats)) {
        int p = nats->value; 
        primes = insert(primes, p);

        int multiple = p;
        while (multiple <= N) {
            nats = deleteOnce(nats, multiple);
            multiple += p;
        }
    }

    return primes;
}

void printInts(TSortedList list) {
    while (!isEmpty(list)) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    printInts(getPrimes(100));
    return 0;
}
