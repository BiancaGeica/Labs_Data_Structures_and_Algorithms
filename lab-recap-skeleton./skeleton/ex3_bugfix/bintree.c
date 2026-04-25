#include "bintree.h"

BTNode *btCreate(int value) {
    BTNode *n = (BTNode *)malloc(sizeof(BTNode));
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

BTNode *btInsert(BTNode *root, int value) {
    if (root == NULL)
        return btCreate(value);
    if (value < root->value)
        root->left = btInsert(root->left, value);
    else if (value > root->value)
        root->right = btInsert(root->right, value);
    return root;
}

int btSize(BTNode *root) {
    if (root == NULL)
        return 0;
    return 1 + btSize(root->left) + btSize(root->right);
}

void btInorderFill(BTNode *root, int *out, int *idx) {
    if (root == NULL)
        return;
    btInorderFill(root->left, out, idx);
    out[(*idx)++] = root->value;
    btInorderFill(root->right, out, idx);
}

void btMirror(BTNode *root) {
    if (root == NULL)
        return;
    BTNode *tmp = root->left;
    root->left = root->right;
    root->right = tmp;
    btMirror(root->left);
    btMirror(root->right); //AM ADAUGAT SI OGLINDIREA SUBARBORELUI DREPT
}

void btFreeTree(BTNode **root) {
    if (*root == NULL)
        return;
    btFreeTree(&(*root)->left);
    btFreeTree(&(*root)->right);
    free(*root);
    *root = NULL;
}
