#include <stdio.h>
#include <stdlib.h>
#include "input.h"

typedef struct AVLTreeNode Node;
struct AVLTreeNode {
    int value;
    int height;

    Node *left;
    Node *right;
};
// Node constructor
void make_node(int value, Node *node) {
    node->value = value;
    node->height = 1;

    node->left = NULL;
    node->right = NULL;
}
// free dynamic memory from tree nodes data
void free_tree(Node *root) {
    if (root->left != NULL) {
        free_tree(root->left);
    }
    if (root->right != NULL) {
        free_tree(root->right);
    }
    free(root);
}
int get_height(Node *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}
// setting correct height to node
void fix_height(Node *node) {
    int height_left = get_height(node->left);
    int height_right = get_height(node->right);
    node->height = (height_left > height_right ? height_left : height_right) + 1;
}
// returns height difference
// between left and right subtrees
int get_diff(Node *node) {
    return get_height(node->left) - get_height(node->right);
}
Node *small_left_rotate(Node *root) {
    Node *right = root->right;
    root->right = right->left;
    right->left = root;
    // calculate new height for nodes
    fix_height(root);
    fix_height(right);
    // return balanced subtree root
    return right;
}
Node *small_right_rotate(Node *root) {
    Node *left = root->left;
    root->left = left->right;
    left->right = root;
    // calculate new height for nodes
    fix_height(root);
    fix_height(left);
    // return balanced subtree root
    return left;
}
Node *big_left_rotate(Node *root) {
    root->right = small_right_rotate(root->right);
    return small_left_rotate(root);
}
Node *big_right_rotate(Node *root) {
    root->left = small_left_rotate(root->left);
    return small_right_rotate(root);
}
// balancing node
Node *balance(Node *node) {
    // default value of root node is input node
    Node *result = node;
    // left rotates
    if (get_diff(node) == -2) {
        if (get_diff(node->right) == 1) {
            result = big_left_rotate(node);
        }
        else {
            // the height difference isn't equal 1 so it can be 0 or -1
            result = small_left_rotate(node);
        }
    }
    else {
        // right rotates (symmetrically with respect to left rotates)
        if (get_diff(node) == 2) {
            if (get_diff(node->left) == -1) {
                result = big_right_rotate(node);
            }
            else {
                result = small_right_rotate(node);
            }
        }
    }
    return result;
}
// returns the root of AVL tree
// after inserting new node
Node *insert(Node *root, Node *paste_node) {
    if (root == NULL) {
        // no nodes in tree yet
        return paste_node;
    }
    if (paste_node->value > root->value) {
        if (root->right == NULL) {
            root->right = paste_node;
        }
        else {
            root->right = insert(root->right, paste_node);
        }
        fix_height(root);
    }
    else {
        if (root->left == NULL) {
            root->left = paste_node;
        }
        else {
            root->left = insert(root->left, paste_node);
        }
        fix_height(root);
    }
    return balance(root);
}

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    input = set_input(argc, argv);
    unsigned int nodes_number = 0;

    if (!fscanf(input, "%u", &nodes_number)) {
        printf("can't read an integer value");
        exit(1);
    }
    Node *root = NULL;
    unsigned int i = 0;
    int value = 0;
    Node *node_list = (Node*) malloc(sizeof(Node) * nodes_number);

    for (; i < nodes_number; i++) {
        if (!fscanf(input, "%d", &value)) {
            printf("can't read an integer value");
            exit(1);
        }
        make_node(value, &node_list[i]);
        root = insert(root, &node_list[i]);
    }
    printf("%d", get_height(root));
    free(node_list);
    return 0;
}
