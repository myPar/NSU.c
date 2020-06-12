#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include <stdint.h>
#include <assert.h>
#include "free_tree.h"
#include "debugging.h"

void check_byte_update(char *byte, int *bit_pos, FILE *input) {
    if (*bit_pos > 7) {
        *bit_pos = 0;
        fread(byte, 1, 1, input);
    }
}

char read_character(FILE *input, char *byte, int *bit_pos) {
    char result = 0;
    // write next 8 bit in character byte

    for (int i = 0; i < 8; i++) {
        if ((byte[0] & (1 << bit_pos[0])) != 0) {
            result |= (1 << i);
        }
        bit_pos[0]++;
        check_byte_update(byte, bit_pos, input);
    }
    return result;
}

Node *tree_restore(FILE *input, char *byte, int *bit_pos) {
    if ((byte[0] & (1 << bit_pos[0])) != 0) {
        // restore node
        Node *node = make_node(-1);
        bit_pos[0]++;
        check_byte_update(byte, bit_pos, input);
        // restore left subtree
        node->left = tree_restore(input, byte, bit_pos);
        // restore right subtree
        node->right = tree_restore(input, byte, bit_pos);
        // return node
        return node;
    }
    else {
        bit_pos[0]++;                                   // read zero bit
        check_byte_update(byte, bit_pos, input);
        char ch = read_character(input, byte, bit_pos); // read character
        Node *node = make_node(ch);
        // return leaf
        return node;
    }
}

void decode_bytes(FILE *input, FILE *output, Node *root, int count) {
    int byte_pos = 0;
    int bit_pos = 8;
    int idx = 0;
    int decoded_count = 0;

    const int max_size = 128;
    int input_buf_size = 0;
    char *input_buffer = (char*) malloc(sizeof(char) * max_size);
    char *output_buffer = (char*) malloc(sizeof(char) * max_size);

    Node *current_node = root;

    while (decoded_count < count) {
        while (current_node->left != NULL) {
            if (byte_pos >= input_buf_size - 1 && bit_pos > 7) {
                byte_pos = 0;
                bit_pos = 0;
                input_buf_size = fread(input_buffer, 1, max_size, input);   // update buffer)
            }
            if (bit_pos > 7) {  // the byte is full
                byte_pos++;
                bit_pos = 0;
            }
            if ((input_buffer[byte_pos] & (1 << bit_pos)) != 0) {
                current_node = current_node->right;                 // bit value - 1
            }
            else {
                current_node = current_node->left;                  // bit value - 0
            }
            bit_pos++;
        }
        decoded_count++;

        if (idx < max_size - 1) {
            output_buffer[idx] = (char) current_node->value;
            idx++;
        }
        else {
            output_buffer[idx] = (char) current_node->value;
            fwrite(output_buffer, sizeof(char), idx + 1,  output);
            output_buffer[0] = '\0';
            idx = 0;
        }
        current_node = root;
    }
    if (idx != 0) {     // add remaining buffer data to output file
        fwrite(output_buffer, sizeof(char), idx, output);
    }

    free(input_buffer);
    free(output_buffer);
}

void decompress(FILE * input, FILE *output) {
    archive_header header = {0};
    read_header(input, &header);

    if (header.alphabet_size > 1) {
        char ch = 0;
        int bit_pos = 0;
        fread(&ch, 1, 1, input);
        Node *root = tree_restore(input, &ch, &bit_pos);
        //assert(root == NULL);
        decode_bytes(input, output, root, header.data_size);
        free_tree(root);
    }
    else {
        unsigned char ch = 0;
        fread(&ch, 1, 1, input);

        for (unsigned i = 0; i < header.data_size; i++) {
            fwrite(&ch, 1, 1, output);
        }
    }
}
