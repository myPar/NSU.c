#include "structures.h"

void free_tree(Node *root) {
    if (root->left == NULL) {
        free(root);
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
