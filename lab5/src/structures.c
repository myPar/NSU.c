#include "structures.h"

Element* make_element(int val, int count) {
    Element *element = (Element*) malloc(sizeof(Element));
    Node* node = make_node(val);
    element->next = NULL;
    element->node = node;
    element->count = count;

    return element;
}

Node* make_node(int val) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->left = NULL;
    node->right = NULL;
    node->value = val;

    return node;
}
