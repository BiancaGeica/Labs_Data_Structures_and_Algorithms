#ifndef AVL_H_
#define AVL_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct avlnode {
    int value;
    int height;
    struct avlnode *left;
    struct avlnode *right;
} *AvlTree;

AvlTree avlCreate(int value);
int avlMax(int a, int b);
int avlHeight(AvlTree root);
int avlBalanceFactor(AvlTree root);
void avlUpdateHeight(AvlTree root);
AvlTree avlLeftRotation(AvlTree x);
AvlTree avlRightRotation(AvlTree y);
AvlTree avlInsert(AvlTree root, int value);
AvlTree avlFreeTree(AvlTree root);

#endif
