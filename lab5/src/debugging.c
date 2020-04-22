#include "structures.h"
#include <stdio.h>
#include <string.h>

void print_tree(Node *root) {
    if (root->value == -1) {
        printf("%s", "node(");
        print_tree(root->left);
        printf("%s", " , ");
        print_tree(root->right);
        printf("%s", ")");
    }
    else {
        char ch = (char)root->value;

        if (ch == '\n') {
            printf("%s", "/n");
        }
        else {
            printf("%c", ch);
        }
        return;
    }
}

void print_queue(Queue *queue, char *format) {
    Element *head = queue->head;

    if (strcmp(format, "count") == 0) {
        while(head != NULL) {
            printf("%d%s", head->count, " ");
            head = head->next;
        }
    }
    else {
        while(head != NULL) {
            char ch = (char)(head->node)->value;

            if (ch == '\n') {
                printf("%s%s", "/n", " ");
            }
            else {
                printf("%c%s", ch, " ");
            }
            head = head->next;
        }
    }
    printf("\n");
}

void print_code(char *code_list[256]) {
    int i = 0;

    for (i = 0; i < 255; i++) {
        if (code_list[i] != NULL) {
            if (i == '\n') {
                printf("%s%s%s\n", "/n", " : ", code_list[i]);
            }
            else {
                printf("%c%s%s\n", i, " : ", code_list[i]);
            }
        }
    }
}

void print_alphabet(char *alphabet) {
    int size = strlen(alphabet);

    for(int i = 0; i < size; i++) {
        if (alphabet[i] == '\n') {
            printf("%s ", "/n");
        }
        else {
            printf("%c ", alphabet[i]);
        }
    }
    printf("\n");
}

int get_bit(char ch, int pos) {
    return (ch & (1 << pos)) != 0;
}

void print_bits(char ch) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", get_bit(ch, i));
    }
    printf("\n");
}

void print_buffer_bit(char *buffer, int count) {
    for (int i = 0; i < count; i++) {
        print_bits(buffer[i]);
    }
}