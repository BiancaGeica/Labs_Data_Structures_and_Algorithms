#include "avl.h"

AvlTree avlCreate(int value) {
    AvlTree node = (AvlTree)malloc(sizeof(struct avlnode));
    node->value = value;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int avlMax(int a, int b) {
    return (a > b) ? a : b;
}

int avlHeight(AvlTree root) {
    if (root == NULL)
        return -1;
    return root->height;
}

int avlBalanceFactor(AvlTree root) { //la avl, balance-factor-ul poate fi doar -1, 0, 1, altfel trebuie recalibrat
    if (root == NULL)
        return 0;
    return avlHeight(root->right) - avlHeight(root->left);
}

void avlUpdateHeight(AvlTree root) {
    if (root == NULL)
        return;
    root->height = 1 + avlMax(avlHeight(root->left), avlHeight(root->right));
}

AvlTree avlLeftRotation(AvlTree x) {
    AvlTree y = x->right;
    x->right = y->left;
    y->left = x;
    avlUpdateHeight(x); //AM ADAUGAT UPDATE HEIGHT UL PE CEALALTA RAMURA
    avlUpdateHeight(y);
    return y;
}

AvlTree avlRightRotation(AvlTree y) {
    AvlTree x = y->left;
    y->left = x->right;
    x->right = y;
    avlUpdateHeight(y);
    avlUpdateHeight(x);
    return x;
}

AvlTree avlInsert(AvlTree root, int value) {
    if (root == NULL)
        return avlCreate(value);
    if (value < root->value)
        root->left = avlInsert(root->left, value);
    else if (value > root->value)
        root->right = avlInsert(root->right, value);
    else
        return root;
    avlUpdateHeight(root);
    int bf = avlBalanceFactor(root);
    if (bf < -1 && value < root->left->value)
        return avlRightRotation(root);
    if (bf < -1 && value > root->left->value) {
        root->left = avlLeftRotation(root->left);
        return avlRightRotation(root);
    }
    if (bf > 1 && value > root->right->value)
        return avlLeftRotation(root);
    if (bf > 1 && value < root->right->value) {
        root->right = avlRightRotation(root->right);
        return avlLeftRotation(root);
    }
    return root;
}

AvlTree avlFreeTree(AvlTree root) {
    if (root == NULL)
        return NULL;
    avlFreeTree(root->left);
    avlFreeTree(root->right);
    free(root);
    return NULL;
}
