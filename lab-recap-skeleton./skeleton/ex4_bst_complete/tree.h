#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef int Item;

typedef struct node {
    Item value;
    struct node *left;
    struct node *right;
} TreeNode, *Tree;

/* Primitive de baza pentru BST (sunt deja implementate). */
Tree bstCreate(Item value);
Tree bstInsert(Tree root, Item value);
void bstDestroy(Tree *root);

/* Functii care trebuie implementate in bst_utils.c. */
int isBST(Tree root);
int kthSmallest(Tree root, int k);

#endif
