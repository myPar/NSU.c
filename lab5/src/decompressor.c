#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include <stdint.h>
#include <assert.h>
#include "free_tree.h"

void get_alphabet(FILE *input, char alphabet[256], size_t size) {
    fread(alphabet, 1, size, input);
}

void check_byte_update(char *byte, int *bit_pos, FILE *input) {
    if (*bit_pos > 7) {
        *bit_pos = 0;
        fread(byte, 1, 1, input);
    }
}

Node* restore_tree(FILE *input, char *alphabet, int *idx, char *byte, int *bit_pos, int* has_leaf, int level) {
    if ((byte[0] & (1 << *bit_pos)) == 0 && *has_leaf == -1) {
        *has_leaf = 1;
        idx[0]++;
        bit_pos[0]++;
        check_byte_update(byte, bit_pos, input);

        Node *node = make_node(alphabet[*idx]);     // make leaf
        return node;
    }
    Node *node = make_node(-1);     // make node

    if ((byte[0] & (1 << *bit_pos)) != 0) {
        *has_leaf = -1;
        bit_pos[0]++;
        check_byte_update(byte, bit_pos, input);

        node->left = restore_tree(input, alphabet, idx, byte, bit_pos, has_leaf, level + 1);
    }
    *has_leaf = -1;
    node->right = restore_tree(input, alphabet, idx, byte, bit_pos, has_leaf, level + 1);

    if (level > 0) {
        bit_pos[0]++;
        check_byte_update(byte, bit_pos, input);
    }

    return node;
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
    //FILE *header_file = fopen("../../header_file", "rb");
    archive_header header = {0};
    read_header(input, &header);

    if (header.alphabet_size > 1) {
        char alphabet[256];
        get_alphabet(input, alphabet, header.alphabet_size);

        int val, val2 = -1;
        int *idx = &val;
        int *has_leaf = &val2;
        *idx = -1;

        char ch = 0;
        int bit_pos = 0;
        fread(&ch, 1, 1, input);
        Node *root = restore_tree(input, alphabet, idx, &ch, &bit_pos, has_leaf, 0);

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