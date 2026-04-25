#include <limits.h>
#include "tree.h"

int isBST_Helper(Tree root, long min, long max) {
    if (root == NULL) {
        return 1;
    }

    if (root->value <= min || root->value >= max) {
        return 0;
    }

    return isBST_Helper(root->left, min, root->value) && isBST_Helper(root->right, root->value, max);
}

int isBST(Tree root) {
    return isBST_Helper(root, LONG_MIN, LONG_MAX);
}

int inorder_index(Tree root, int *idx, int k) {
    if (root == NULL) {
        return INT_MIN;
    }

    int stanga = inorder_index(root->left, idx, k);
    if (stanga != INT_MIN) {
        return stanga; 
    }

    (*idx)++;
    if (*idx == k) {
        return root->value;
    }

    return inorder_index(root->right, idx, k);
}

int kthSmallest(Tree root, int k) {
    // TODO: al k-lea element din inordine; oprire imediata la gasire.
    (void)root;
    (void)k;

    if (root == NULL)
        return INT_MIN;

    int count = 0;
    return inorder_index(root, &count, k);
}
