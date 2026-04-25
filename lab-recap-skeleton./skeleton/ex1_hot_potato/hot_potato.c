#include "hot_potato.h"

typedef int Item;
#include "Queue.h"

int hotPotato(int N, int k, int *eliminationOrder) {
    if (N < 1 || k < 1)
        return -1;

    // TODO: implementeaza jocul folosind o coada (Queue.h).
    (void)eliminationOrder;

    if (N == 1) {
        return 1;
    }

    Queue *players_queue = createQueue();
    for (int i = 1; i <= N; i++) {
        enqueue(players_queue, i);
    }

    int number_losers = 0;
    while (!isQueueEmpty(players_queue)) {
        for (int i = 0; i < k - 1; i++) {
            int current_player = front(players_queue);
            dequeue(players_queue);
            enqueue(players_queue, current_player);
        }
        int current_player = front(players_queue);
        dequeue(players_queue);
        eliminationOrder[number_losers++] = current_player;
    }

    destroyQueue(players_queue);
    return eliminationOrder[--number_losers];
}
