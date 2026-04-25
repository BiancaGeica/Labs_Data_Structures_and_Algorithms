#include "tree.h"

Tree bstCreate(Item value) {
    Tree n = (Tree)malloc(sizeof(TreeNode));
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Tree bstInsert(Tree root, Item value) {
    if (root == NULL)
        return bstCreate(value);
    if (value < root->value)
        root->left = bstInsert(root->left, value);
    else if (value > root->value)
        root->right = bstInsert(root->right, value);
    return root;
}

void bstDestroy(Tree *root) {
    if (*root == NULL)
        return;
    bstDestroy(&(*root)->left);
    bstDestroy(&(*root)->right);
    free(*root);
    *root = NULL;
}
