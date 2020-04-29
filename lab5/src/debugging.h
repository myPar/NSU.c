#ifndef LAB_HAFFMAN_DEBUGGING_H
#define LAB_HAFFMAN_DEBUGGING_H
#include "structures.h"
void print_tree(Node *root);
void print_queue(Queue *queue, char *format);
void print_code(char *code_list[256]);
void print_bits(char ch);
void print_buffer_bit(char *buffer, int count);
void print_alphabet(char *alphabet);

#endif //LAB_HAFFMAN_DEBUGGING_H