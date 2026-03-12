#ifndef _PALINDROME_H_
#define _PALINDROME_H_

/* Palindrome computation function
 * Determina daca o lista dublu inlantuita de caractere formeaza un palindrom.
 * Foloseste un singur ciclu. NU se foloseste memorie auxiliara.
 * Returneaza 1 daca e palindrom, 0 altfel.
 */
int isPalindrome(List* list) {
	// TODO: Cerinta 2
	ListNode *head = list->first;
	ListNode *tail = list->last;

	if(list == NULL || isEmpty(list))
		return 1;

	while (head->next != NULL && tail->prev != NULL) {
		if (head->elem != tail->elem) 
			return 0;
		head = head->next;
		tail = tail->prev;
	}
	return 1;
}

#endif
