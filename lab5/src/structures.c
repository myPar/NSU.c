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

void write_header(FILE *f, archive_header *header) {
    fwrite(header, 1, sizeof(archive_header), f);
}

void read_header(FILE *f, archive_header *header) {
    if (!fread(header, 1, sizeof(archive_header), f)) {
        printf("can't read header\n");
        exit(1);
    }
}

void print_header(archive_header *header) {
    printf("header = (alphabet size: %u , data size: %d)\n",
           header->alphabet_size, header->data_size);
}
/*
fpos_t get_cur_file_pos(FILE *f) {
    fpos_t pos = 0;

    if (fgetpos(f, &pos) != 0) {
        printf("ERROR: fgetpos failed\n");
        exit(1);
    }
    return pos;
}
*/