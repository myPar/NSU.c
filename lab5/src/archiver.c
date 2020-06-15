#include "structures.h"
#include "free_tree.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Element* insert(Queue *queue, Element *element) {

    if (queue->head == NULL) {  // empty queue case
        queue->head = element;
    }
    else {
        if (element->count <= (queue->head)->count) {
            element->next = queue->head;
            queue->head = element;
            return queue->head;
        }

        Element *node = queue->head;
        while (node->next != NULL) {
            if (element->count > node->count && element->count <= (node->next)->count) {
                element->next = node->next;
                node->next = element;
                break;
            }
            node = node->next;
        }

        if (node->next == NULL) {
            node->next = element;
        }
    }

    return queue->head;
}
// gets array of symbols count in the text; returns number of chars in the text
int get_count(FILE *input, int *count_array) {
    unsigned char buffer[128];
    const int max_size = sizeof(buffer);
    int count = 0;

    while(!feof(input)) {
        size_t buffer_size = fread(buffer, 1, max_size, input);

        for (int i = 0; i < (int) buffer_size; i++) {
            count_array[buffer[i]]++;
            count++;
        }
    }
    return count;
}

Queue* build_queue(int *count_array) {
    Queue *queue = (Queue*) malloc(sizeof(Queue));  // allocate space in dynamic memory
    queue->head = NULL;

    for (int i = 0; i < 256; i++) {
        if (count_array[i] > 0) {
            Element *element = make_element(i, count_array[i]);
            insert(queue, element);
        }
    }
    return queue;
}

Node* build_tree(Queue *queue) {
    Element *element = queue->head;

    while (element->next != NULL) {
        Element *parent = make_element(-1, element->count + (element->next)->count);    // make parent
        (parent->node)->left = element->node;
        (parent->node)->right = (element->next)->node;

        queue->head = (element->next)->next;    // change head pointer

        free(element->next);
        free(element);

        insert(queue, parent);
        element = queue->head;
    }
    Node *root = element->node;
    free(element);
    free(queue);
    return root;   // return root
}

void get_code(FILE *output, Node *root, char code[257], int level, char *code_list[256]) {
    if (root->left == NULL) {    // leaf case
        code_list[root->value] = strcpy((char*)malloc(sizeof(char) * level + 1), code);
    }
    else {
        code[level] = '0';
        code[level + 1] = '\0';
        get_code(output, root->left, code, level + 1, code_list);
        code[level] = '1';
        code[level + 1] = '\0';
        get_code(output, root->right, code, level + 1, code_list);
    }
}

void check_bit(int *bit_pos, int *byte_pos, char *output_buffer) {
    if (bit_pos[0] > 7) {
        bit_pos[0] = 0;
        byte_pos[0] += 1;
        output_buffer[*byte_pos] = 0;   // null bits in next byte
    }
}

void write_tree(FILE *output, Node *root, char *output_buffer, int *byte_pos, int *bit_pos) {
    if (root->left == NULL) {
        // write leaf
        (*bit_pos)++;
        check_bit(bit_pos, byte_pos, output_buffer);
        // next 8 bits will current alphabet character's bits
        unsigned char ch = (unsigned char) root->value;

        for (int i = 0 ; i < 8; i++) {
            if ((ch & (1 << i)) != 0) {
                output_buffer[*byte_pos] |= (1 << *bit_pos);
            }
            (*bit_pos)++;
            check_bit(bit_pos, byte_pos, output_buffer);
        }
        return;
    }
    // write node
    output_buffer[*byte_pos] |= (1 << *bit_pos);
    (*bit_pos)++;
    check_bit(bit_pos, byte_pos, output_buffer);
    // go to left sub tree
    write_tree(output, root->left, output_buffer, byte_pos, bit_pos);
    // go to right sub tree
    write_tree(output, root->right, output_buffer, byte_pos, bit_pos);
}

void write_bytes(FILE* input, FILE *output, char *code_list[256]) {
#define max_size 128
// max_size should be enough for output buffer because average code size must be
    char *input_buffer = (char*) malloc(sizeof(char) * max_size);
    char output_buffer[max_size] = {0};
    int byte_pos = -1;
    int bit_pos = 8;
    int buf_count = 0;

    while(!feof(input)) {
        size_t buffer_size = fread(input_buffer, 1, max_size, input);
        buf_count++;

        for (unsigned i = 0; i < buffer_size; i++) {
            unsigned char ch = input_buffer[i];
            int code_len = strlen(code_list[ch]);

            for (int j = 0; j < code_len; j++) {
                if (byte_pos >= max_size - 1 && bit_pos > 7) {      // the buffer is full
                    fwrite(output_buffer, 1, max_size, output);
                    byte_pos = -1;
                    bit_pos = 8;
                }
                if (bit_pos > 7) {  // byte is full
                    bit_pos = 0;
                    byte_pos++;
                    output_buffer[byte_pos] = 0;     // null bits in new byte
                }

                if (code_list[ch][j] == '1') {
                    output_buffer[byte_pos] |= (1 << bit_pos);  // set 1 in bit with "bit_pos" position in the byte
                }
                bit_pos++;
            }
        }
        input_buffer[0] = '\0';
    }

    if (byte_pos >= 0) {     // add remaining buffer data to output file
        fwrite(output_buffer, 1,byte_pos + 1, output);    // write byte ot output file
    }
    free(input_buffer);
#undef max_size
}

#define size 256
#define size_ 257
#define max_tree_size 500

void archive(FILE *input, FILE *output) {
    int count;
    int count_array[size] = {0};
    count = get_count(input, count_array);

    if (count > 0) {
        fwrite(&count, sizeof(int), 1, output);

        Queue *queue = build_queue(count_array);
        Node *root = build_tree(queue);

        char *code_list[size] = {NULL};
        char code[size_];
        code[0] = '\0';
        get_code(output, root, code, 0, code_list);

        char tree_reverse_buffer[max_tree_size] = {0};
        tree_reverse_buffer[0] = 0;     // init first buffer byte
        int byte_pos = 0;
        int bit_pos = 0;
        write_tree(output, root, tree_reverse_buffer, &byte_pos, &bit_pos);
        fwrite(tree_reverse_buffer, 1, byte_pos + 1, output);

        if (root->left != NULL) {
            fseek(input, 0, SEEK_SET);  // sets start position in input file
            write_bytes(input, output, code_list);
        }

        free_code_list(code_list);
        free_tree(root);
    }
}

#undef size
#undef size_
#undef max_tree_size
