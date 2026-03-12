#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new node
 *  [input]: Item
 *  [output]: List*
 */
ListNode *createNode(Item elem) {
	// TODO: Cerinta 1a
	ListNode* node = (ListNode*) malloc (sizeof(ListNode));
	if(node == NULL) 
		return node;

	node->elem = elem;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
	// TODO: Cerinta 1a
	List *list = (List*) malloc (sizeof(List));
	list->first = list->last = NULL;

	return list;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	// TODO: Cerinta 1b
	if(list == NULL) //segfault daca nu se verifica pentru ca incearca sa acceseze 0x0 si nu are acces
		return 1;
	if(list->first == NULL && list->last == NULL)
		return 1;
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
	// TDOO: Cerinta 1c
	ListNode* node = list->first;
	while (node != NULL){
		if (node->elem == elem) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){
    if(list == NULL || pos < 0) return;

    if (isEmpty(list)) {
        if (pos == 0) {
            ListNode *node = createNode(elem);
            if (node == NULL) return;
            list->first = node;
            list->last = node;
        }
        return; 
    }

    ListNode *node = createNode(elem);
    if (node == NULL) return;

    if (pos == 0) { 
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
        return;
    }

    ListNode *temp = list->first;
    int index = 0;
    while (temp->next != NULL && index < pos - 1) {
        temp = temp->next;
        index++;
    }

    if (index < pos - 1) {
        free(node);
        return;
    }

    node->next = temp->next;
    node->prev = temp;

    if (temp->next != NULL) {
        temp->next->prev = node;
    } else {
        list->last = node;
    }
    temp->next = node;
}
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
    if(list == NULL || isEmpty(list)) return;

    if (list->first->elem == elem){ 
        ListNode* temp = list->first;
        list->first = list->first->next;
        if (list->first != NULL) {
            list->first->prev = NULL;
        } else {
            list->last = NULL;
        }
        free(temp);
        return;
    }

    ListNode* temp = list->first;
    while (temp != NULL && temp->elem != elem) {
        temp = temp->next;
    }

    if (temp != NULL) {
        if (temp == list->last) {
            list->last = temp->prev;
            if (list->last != NULL) {
                list->last->next = NULL;
            } else {
                list->first = NULL;
            }
        } else {
            temp->prev->next = temp->next;
            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            }
        }
        free(temp);
    }
}
// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;

	// TODO: Cerinta 1f
	int len = 0;
	ListNode *temp = list->first;
	while(temp != NULL) {
		len++;
		temp = temp->next;
	}
	return len;
}
// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List* destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return NULL;

	//TODO: Cerinta 1g
	ListNode *current = list->first;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }

    free(list);
    return NULL;
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
