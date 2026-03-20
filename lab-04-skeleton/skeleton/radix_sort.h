#ifndef RADIX_SORT_H_
#define RADIX_SORT_H_

#include "Queue.h"

int arrayMaximum(const int* arr, int len){
    int max = arr[0];
    for(int i = 1; i < len; i++){
        if(arr[i] > max)
            max = arr[i];
    }
    return max;
}

int getNumberOfDigits(int num){
    if (num == 0) return 1;
    int digits = 0;
    while(num){
        num = num/10;
        digits++;
    }
    return digits;
}

void radixSort(int *arr, int len){

    // TODO: Cerinta 4 (Bonus) - Implementation must use queues
    // 1. Initialize queues for each digit, i.e. our buckets
    Queue *buckets[10];
    for (int i = 0; i < 10; i++) {
        buckets[i] = createQueue();
    }

    // 2. Compute number of iterations to sort the array:
    // 2a. Compute array maximum
    int max_number = arrayMaximum(arr, len);
    // 2b. Compute maximum number of digits
    int max_digits = getNumberOfDigits(max_number);

    // Radix sort
    // 3. Iterate over the maximum number of digits
    int exp = 1;
    for (int d = 0; d < max_digits; d++) {
        // 3a. Place the number in a bucket according to its digit
        for (int i = 0; i < len; i++) {
            int digit = (arr[i] / exp) % 10;
            enqueue(buckets[digit], arr[i]);
        }

        // 3b. Re-construct the array by using each bucket in order
        int idx = 0;
        for (int i = 0; i < 10; i++) {
            while (!isQueueEmpty(buckets[i])) {
                arr[idx++] = front(buckets[i]);
                dequeue(buckets[i]);
            }
        }
        exp *= 10;
    }

    //4. Destroy queues
    for (int i = 0; i < 10; i++) {
        destroyQueue(buckets[i]);
    }
}

#endif