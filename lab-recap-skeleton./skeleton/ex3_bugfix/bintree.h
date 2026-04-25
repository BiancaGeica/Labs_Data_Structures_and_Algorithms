#ifndef BINTREE_H_
#define BINTREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct btnode {
    int value;
    struct btnode *left;
    struct btnode *right;
} BTNode;

BTNode *btCreate(int value);
BTNode *btInsert(BTNode *root, int value);
int btSize(BTNode *root);
void btInorderFill(BTNode *root, int *out, int *idx);
void btMirror(BTNode *root);
void btFreeTree(BTNode **root);

#endif
