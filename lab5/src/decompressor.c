#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include <stdint.h>
#include <assert.h>
#include "free_tree.h"

void get_alphabet(FILE *input, char alphabet[256], size_t size) {
    fread(alphabet, 1, size, input);
}

Node* restore_tree(FILE *input, char *alphabet, int *idx, int ch, int* has_leaf, int depth) {
    if (ch == 'U' && *has_leaf == -1) {
        *has_leaf = 1;
        idx[0] += 1;
        return make_node(alphabet[*idx]);       // make leaf
    }

    Node *node = make_node(-1);     // make node
    if (ch == 'L') {
        *has_leaf = -1;
        node->left = restore_tree(input, alphabet, idx, getc(input), has_leaf, depth + 1);     // gets left sub tree
    }

    ch = getc(input);
    if (ch == 'R') {
        *has_leaf = -1;
        node->right = restore_tree(input, alphabet, idx, getc(input), has_leaf, depth + 1);    // gets right sub tree
    }
    if (depth > 0) {
        getc(input);    // read 'U' before returning the node
    }
    return node;    // return node
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
    FILE *header_file = fopen("../../header_file", "rb");
    archive_header header = {0};
    read_header(header_file, &header);

    if (header.alphabet_size > 1) {
        char alphabet[256];
        get_alphabet(header_file, alphabet, header.alphabet_size);

        int val, val2 = -1;
        int *idx = &val;
        int *has_leaf = &val2;
        *idx = -1;

        Node *root = restore_tree(header_file, alphabet, idx, getc(header_file), has_leaf, 0);
/*        assert((get_cur_file_pos(input) == (sizeof(archive_header) + header.alphabet_size + header.tree_size)) &&
               "wrong pos after reading tree");
*/
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
    fclose(header_file);
    remove("../../header_file");
}