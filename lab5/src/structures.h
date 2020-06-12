#ifndef LAB_HAFFMAN_STRUCTURES_H
#define LAB_HAFFMAN_STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct st_Node Node;
struct st_Node {       // node in tree
    Node *left;
    Node *right;

    int value;
};
Node* make_node(int val);

typedef struct st_Element Element;
struct st_Element {
    Element *next;
    Node *node;
    int count;          // symbol count in the input text
};
Element* make_element(int val, int count);

typedef struct Linked_list Queue;
struct Linked_list {
    Element *head;
};

typedef struct {
    unsigned char alphabet_size;
    int data_size;
} archive_header;

void write_header(FILE *f, archive_header *header);
void read_header(FILE *f, archive_header *header);
void print_header(archive_header *header);
fpos_t get_cur_file_pos(FILE *f);
#endif //LAB_HAFFMAN_STRUCTURES_H
