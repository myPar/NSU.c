#include "archiver.h"
#include "structures.h"
#include "free_tree.h"
#include "util.h"

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

        for (int i = 0; i < buffer_size; i++) {
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
        fputc(root->value, output);         // write alphabet in order of
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

void tree_write(FILE *output, Node *root, int level, char *output_buffer, int *byte_pos, int *bit_pos) {
    if (root->left == NULL) {
        bit_pos[0] += 1;
        check_bit(bit_pos, byte_pos, output_buffer);
        return;
    }
    // equal writing 'L'
    output_buffer[*byte_pos] |= (1 << *bit_pos);
    bit_pos[0]++;
    check_bit(bit_pos, byte_pos, output_buffer);
    tree_write(output, root->left, level + 1, output_buffer, byte_pos, bit_pos);
    // equal writing 'R'
    tree_write(output, root->right, level + 1, output_buffer, byte_pos, bit_pos);
    // write 'U' after traverse of right subtree
    if (level > 0) {
        bit_pos[0]++;
        check_bit(bit_pos, byte_pos, output_buffer);
    }
    else {
        assert(level == 0);
        // write tree traverse in the output file
        fwrite(output_buffer, 1, *byte_pos + 1, output);
    }
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

void archive(FILE *input, FILE *output) {
    unsigned char ch = 0;
    int alphabet_size = 0;
    int count;
    int count_array[256] = {0};
    count = get_count(input, count_array);

    for (int i = 0; i < 256; i++) {
        if (count_array[i] > 0) {
            ch = i;
               alphabet_size++;
        }
    }
    dbg(2, "archive(): count=%d, alphabet_size=%d", count, alphabet_size);

    if (alphabet_size > 1) {
        Queue *queue = build_queue(count_array);
        Node *root = build_tree(queue);

        char *code_list[256] = {NULL};
        char code[257];
        code[0] = '\0';

        archive_header header = {0};
        write_header(output, &header);  // TODO: replace with set pos
        fpos_t start_pos = get_cur_file_pos(output);
        dbg(4, "header start_pos=%llu", start_pos);
        fpos_t cur_pos;

        get_code(output, root, code, 0, code_list);
        cur_pos = get_cur_file_pos(output);
        dbg(4, "header pos after get_code: %llu", cur_pos);
        header.alphabet_size = (size_t)(cur_pos - start_pos);
        start_pos = cur_pos;

        char tree_reverse_buffer[500] = {0};
        tree_reverse_buffer[0] = 0;     // init first buffer byte
        int byte_pos = 0;
        int bit_pos = 0;
        tree_write(output, root, 0, tree_reverse_buffer, &byte_pos, &bit_pos);

        cur_pos = get_cur_file_pos(output);
        dbg(4, "header pos after tree_write: %llu", cur_pos);
        header.tree_size = (size_t)(cur_pos - start_pos);

        fseek(input, 0, SEEK_SET);  // sets start position in input file
#ifdef _DEBUG
        cur_pos = get_cur_file_pos(input);
        dbg(4, "input pos after fseek to 0: %llu", cur_pos);
#endif
        write_bytes(input, output, code_list);

        header.data_size = count;
        // write header
        fseek(output, 0, SEEK_SET);     // move file pointer to the beginning of file
        write_header(output, &header);
        free_tree(root);
        dbg(1, "header { alph=%zu, tree=%zu, data=%zu }", header.alphabet_size, header.tree_size, header.data_size);
    }
    else {
        archive_header header = {1, 0, count};
        write_header(output, &header);
        fwrite(&ch, 1, 1, output);
    }
}
