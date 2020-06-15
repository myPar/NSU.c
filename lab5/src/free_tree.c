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

void free_code_list(char *code_list[256]) {
    for (int i = 0 ; i < 256; i++) {
        if (code_list[i] != NULL) {
            free(code_list[i]);
        }
    }
}
