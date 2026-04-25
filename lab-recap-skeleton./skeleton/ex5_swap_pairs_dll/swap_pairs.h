#ifndef SWAP_PAIRS_H_
#define SWAP_PAIRS_H_

void swapPairs(List *list) {
    // TODO: interschimba fiecare pereche de noduri adiacente, in-place.
    (void)list;

    if (list == NULL || list->first == NULL || list->first->next == NULL) {
        return;
    }

    ListNode *curr = list->first;
    while (curr != NULL && curr->next != NULL) {
        ListNode *nod1 = curr;
        ListNode *nod2 = curr->next;
        ListNode *prev_nod = nod1->prev;
        ListNode *next_nod = nod2->next;

        if (prev_nod != NULL) {
            prev_nod->next = nod2;
        } else {
            list->first = nod2;
        }
        nod2->prev = prev_nod;

        nod1->next = next_nod;
        if (next_nod != NULL) {
            next_nod->prev = nod1;
        }

        nod2->next = nod1;
        nod1->prev = nod2;
        curr = next_nod;
    }

    ListNode *temp = list->first;
    while (temp != NULL && temp->next != NULL) {
        temp = temp->next;
    }
    if (temp != NULL) {
        list->last = temp; 
    }
}

#endif
