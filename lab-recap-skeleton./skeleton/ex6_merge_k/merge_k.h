#ifndef MERGE_K_H_
#define MERGE_K_H_

SortedList mergeKSortedLists(SortedList *lists, int K) {
    if (lists == NULL || K <= 0)
        return NULL;

    APriQueue pq = makeQueue(K > 0 ? K : 1);

    for (int i = 0; i < K; i++) {
        if (lists[i] != NULL) {
            ItemType x;
            x.prior = -lists[i]->value;
            x.data = lists[i];
            heapInsert(pq, x);
        }
    }

    SortedList result = NULL;
    SortedList tail = NULL;

    while (pq->size > 0) {
        ItemType top = getMax(pq);
        removeMax(pq);

        SortedList node = slCreate(top.data->value);
        if (result == NULL) {
            result = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        if (top.data->next != NULL) {
            ItemType nextItem;
            nextItem.prior = -top.data->next->value;
            nextItem.data = top.data->next;
            heapInsert(pq, nextItem);
        }
    }

    freeQueue(pq);
    return result;
}

#endif
