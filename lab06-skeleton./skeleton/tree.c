/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Tree parent, Item value) {
	Tree root = malloc(sizeof(TreeNode));
	root->parent = parent;
	root->value = value;
	root->left = NULL;
	root->right = NULL;
	return root;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Tree parent, Item value) {
	// TODO 0
	*root = (Tree*)malloc(sizeof(Tree));
	if (root == NULL) {
		exit(EXIT_FAILURE);
	}

	(*root)->left = NULL;
	(*root)->right = NULL;
	(*root)->parent = parent;
	(*root)->value = value;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value) {
	// TODO 1
	if (root == NULL) {
		return createTree(NULL, value);
	}

	if (root->value == value) {
		return root;
	} else if (root->value > value) {
		root->left = insert(root->left, value);
		root->left->parent = root;
	} else {
		root->right = insert(root->right, value);
		root->right->parent = root;
	}

	return root;
}


/*
*	Funcție care verifică dacă un arbore este vid sau nu
*		- întoarce 1 dacă arborele este vid
*		- întoarce 0 dacă arborele nu este vid
*/
int isEmpty(Tree root) {
	// TODO 2
	return (root == NULL);
}

/*
*	Funcție care verifică dacă o valoare există sau nu în arbore
*		- 1 dacă value există în arbore
*		- 0 dacă value nu există în arbore
*/
int contains(Tree tree, Item value) {
	// TODO 3
	if (tree == NULL) {
		return 0;
	}
	if(tree->value == value) {
		return 1;
	} else if (tree->value > value) {
		return contains(tree->left, value);
	}
	return contains(tree->right, value);
}

Tree give_node(Tree root, Item value) {
	if (root == NULL) {
		return NULL;
	}
	if(root->value == value) {
		return root;
	} else if (root->value > value) {
		return give_node(root->left, value);
	}
	return give_node(root->right, value);
}

/*
*	Funcție care determină elementul minim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree minimum(Tree tree) {
	// TODO 4.1
	if (tree == NULL || tree->left == NULL) {
		return tree;
	}
	return minimum(tree->left);
}

/*
*	Funcție care determină elementul maxim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree maximum(Tree tree) {
	// TODO 4.2
	if (tree == NULL || tree->right == NULL) {
		return tree;
	}
	return maximum(tree->right);
}

/*
*	Funcție care determină succesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree successor(Tree root, Item value) {
	// TODO 5
	//if (!contains(root, value)) {
	//	return NULL; daca il verific intai pe contains, atunci voi avea o parcurgere redundanta la inceput
	//}
	Tree desired_node = give_node(root, value);

	if (desired_node == NULL) { //nodul cu valoarea dorita nu exista in arbore
		return NULL;
	}

	if (desired_node->right != NULL) {
		return minimum(desired_node->right); //daca avem arbore drept (numere mai mari), cautam minimul din dreapta
	}

	//while(desired_node->parent != NULL && desired_node == desired_node->parent->right) {
	//	desired_node = desired_node->parent;
	//	desired_node->parent = desired_node->parent->parent; modifica arborele cu totul in loc sa il parcurga
	//}

	Tree parent = desired_node->parent;
	while (parent != NULL && desired_node == parent->right) {
		desired_node = parent;
		parent = parent->parent;
	}

	return parent; //parent este NULL daca nu am gasit sau succesorul daca exista
}

/*
*	Funcție care determină predecesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree predecessor(Tree root, Item value) {
	// TODO 6
	Tree desired_node = give_node(root, value);

	if (desired_node == NULL) { //nodul cu valoarea dorita nu exista in arbore
		return NULL;
	}

	if (desired_node->left != NULL) {
		return maximum(desired_node->left); //daca avem arbore stng (numere mai mici), cautam maximul din stanga
	}

	Tree parent = desired_node->parent;
	while (parent != NULL && desired_node == parent->left) {
		desired_node = parent;
		parent = parent->parent;
	}

	return parent;
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void destroyTree(Tree *root) {
	// TODO
	if (*root != NULL) {
		destroyTree(&(*root)->left);
		destroyTree(&(*root)->right);
		free(*root);
	}

	*root = NULL; //root pastreaza vechea adresa de memorie daca nu ii dau NULL aici
}

/*
*	Funcție care șterge un nod cu o anumită valoare din arbore
*/
Tree delete(Tree root, Item value) {
	// TODO 7
	if (root == NULL) {
		return root;
	}
	if (root->value > value) {
		root->left = delete(root->left, value);
	} else if (root->value < value) {
		root->right = delete(root->right, value);
	} else if(root->left != NULL && root->right != NULL) {
		Tree aux = minimum(root->right);
		root->value = aux->value;
		root->right = delete(root->right, aux->value);

		//free(aux);
	} else {
		Tree aux = root;
		if (root->left != NULL) {
			root = root->left;
		} else {
			root = root->right;
		}
		free(aux);
	}
	return root;
}

/*
*	Funcție care determină cel mai apropiat strămoș comun pentru
* două noduri având cheile value1 și value2
*/
Tree lowestCommonAncestor(Tree root, Item value1, Item value2) {
	// TODO 8
	if (value1 < root->value && value2 < root->value) {
		return lowestCommonAncestor(root->left, value1, value2);
	} else if(value1 > root->value && value2 > root->value) {
		return lowestCommonAncestor(root->right, value1, value2);
	}
	return root;
}
