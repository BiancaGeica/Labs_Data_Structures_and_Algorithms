/*
*	Created by Nan Mihai on 11.04.2023
*	Copyright (c) 2023 Nan Mihai. All rights reserved.
*	Laborator 7 - Structuri de date si algoritmi
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2022-2023, Seria CD
*/
#include "avl.h"

/*
*	Functie care creeaza un nod nou cu valoarea primita
*	- height = 0 (frunza)
*	- left = right = NULL
*/
Tree createTree(T value) {
	// TODO 1
	Tree tree = (Tree)malloc(sizeof(struct node));
	if(tree == NULL) {
		exit(EXIT_FAILURE);
	}

	tree->value = value;
	tree->left = NULL;
	tree->right = NULL;
	tree->height = 0;

	return tree;
}

/*
*	Functie care returneaza maximul a doua valori
*/
int max(int a, int b) {
	// TODO 2
	return (a > b) ? a : b;
}

/*
*	Functie care returneaza inaltimea unui arbore
*	- height(NULL) = -1
*/
int height(Tree root) {
	// TODO 3
	if (root == NULL) {
		return -1;
	}
	return root->height;
}

/*
*	Functie care returneaza factorul de echilibru al unui nod
*	BF(root) = height(root->right) - height(root->left)
*/
int balancedFactor(Tree root) {
	// TODO 4
	if (root == NULL)
		return 0;
	
	return height(root->right) - height(root->left);
}

/*
*	Functie care actualizeaza inaltimea unui nod
*	height = 1 + max(height(left), height(right))
*/
void updateHeight(Tree root) {
	// TODO 5
	root->height = 1 + max(height(root->left), height(root->right));
}

/*
*	Functie care realizeaza rotatia stanga
*	Nu uitati sa actualizati inaltimile!
*/
Tree leftRotation(Tree x) {
	// TODO 6
	//Tree tree = (Tree)malloc(sizeof(struct node));
	//if(tree == NULL) {
	//	exit(EXIT_FAILURE);
	//}

	Tree y;

	y = x->right;
	x->right = y->left;
	y->left = x;

	updateHeight(x);
	updateHeight(y);

	return y;
}

/*
*	Functie care realizeaza rotatia dreapta
*	Nu uitati sa actualizati inaltimile!
*/
Tree rightRotation(Tree y) {
	// TODO 7
	//Tree tree = (Tree)malloc(sizeof(struct node));
	//if(tree == NULL) {
	//	exit(EXIT_FAILURE);
	//} MODIFICA MEMORIA CU TOTUL, Strica graful

	Tree z;

	z = y->left;
	y->left = z->right;
	z->right = y;

	updateHeight(y);
	updateHeight(z);

	return z;
}

/*
*	Functie care insereaza o valoare intr-un arbore AVL
*	- Inserare BST standard
*	- Reechilibrare AVL (4 cazuri: LL, LR, RR, RL)
*/
Tree insert(Tree root, T value) {
	// TODO 8
	if(root == NULL) {
		root = createTree(value);
		return root;
	}

	if (value < root->value) {
		root->left = insert(root->left, value);
	} else if (value > root->value) {
		root->right = insert(root->right, value);
	} else
		return root;

	updateHeight(root);

	int bf = balancedFactor(root);

	if (bf < -1 && value < root->left->value)
        return rightRotation(root);

	if (bf > 1 && value > root->right->value)
        return leftRotation(root);

	if (bf < -1 && value > root->left->value) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

	if (bf > 1 && value < root->right->value) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

	return root;
}

/*
*	Functie care determina nodul cu valoarea minima
*	- NULL pentru arbore vid
*/
Tree minimum(Tree root) {
	// TODO 9
	if (root == NULL) {
		return NULL;
	}

	Tree iter = root;
    while (iter->left != NULL) {
        iter = iter->left;
    }

	return iter;
}

/*
*	Functie care sterge un nod cu o anumita valoare din arbore AVL
*	- Stergere BST standard
*	- Reechilibrare AVL
*/
Tree delete(Tree root, T value) {
	// TODO 10
	if (root == NULL)
		return NULL;

	if (value < root->value)
		root->left = delete(root->left, value);
	else if (value > root->value)
		root->right = delete(root->right, value);
	else {
		if (root->left == NULL || root->right == NULL) {
			Tree temp = root;
			if (root->left != NULL) 
				root = root->left;
			else root = root->right;
			free(temp);
		} else {
			Tree temp = minimum(root->right);
			root->value = temp->value;
			root->right = delete(root->right, temp->value);
		}
	}

	if (root == NULL)
		return root;

	updateHeight(root);
	int bf = balancedFactor(root);

	if (bf < -1 && balancedFactor(root->left) <= 0)
        return rightRotation(root);

    if (bf < -1 && balancedFactor(root->left) > 0) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

    if (bf > 1 && balancedFactor(root->right) >= 0)
        return leftRotation(root);

    if (bf > 1 && balancedFactor(root->right) < 0) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

	return root;
}

/*
*	Functie care dealoca intreaga memorie alocata pentru un arbore
*	- Returneaza NULL
*/
Tree freeTree(Tree root) {
	// TODO 11
	if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return NULL;
}
