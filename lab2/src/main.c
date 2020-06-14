#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"

// itoa() isn't valid for builder on gitlab
void int_to_str(int i, char *result) {
    char ch = (char)(i + '0');
    result[0] = ch;
    result[1] = '\0';
}
enum{src_size = 128, que_size = 10, perm_size = 11};

int get_alphabet(int size, const char *input_str, int *queue) {
    for (int i = 0; i < size; i++) {
        int cur_int = input_str[i] - '0';

        if (cur_int > 9 || cur_int < 0) {
            // unexpected character in input string
            return 1;
        }
        queue[cur_int]++;

        if (queue[cur_int] > 1) {
            // input string is not the permutation
            return 1;
        }
    }
    return 0;
}

void get_permutations(int queue[que_size], int queue_size, char *perm_str, int *perm_count,
        char *input_perm, bool *was_comparing) {

    if (*perm_count <= 0) {
        return;//
    }
    // if all numbers from queue was got so
    // building new permutation is complete
    if (queue_size <= 0) {
        // check if current permutation is equal to start permutation
        if (!(*was_comparing)) {
            if (!strcmp(input_perm, perm_str)) {
                *was_comparing = true;
            }
        }
        else {
            // print the permutation
            printf("%s%s", perm_str, "\n");
            (*perm_count)--;
        }
    }
    else {
        char str_number[que_size] = {0};

        for (int i = 0; i < que_size; i++) {
            if (queue[i] != 0) {
                // make new copy for every iteration
                int queue_copy[que_size];
                memcpy(queue_copy, queue, sizeof(int) * que_size);

                char perm_str_copy[perm_size] = {0};
                strcpy(perm_str_copy, perm_str);

                // get i-number for current permutation
                queue_copy[i] = 0;
                // convert i to string for merging it
                // with current permutation string
                int_to_str(i, str_number);
                strcat(perm_str_copy, str_number);
                get_permutations(queue_copy, queue_size - 1, perm_str_copy,
                                 perm_count, input_perm, was_comparing);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *input;

    if ((input = set_input(argc, argv)) == NULL) {
        return 1;
    }
    char input_str[src_size] = {0};
    int queue[que_size] = {0};

    int permutation_count;
    char permutation_str[perm_size] = {0};
    bool was_comparing = false;

    if (fgets(input_str, sizeof(input_str), input) == NULL) {
        printf("can't read the string\n");
        return 1;
    }
    if (!scanf("%d", &permutation_count)) {
        printf("can't read an integer value\n");
        return 1;
    }
    int ch_number = (int) strlen(input_str);

    // cut '\n' in the end of the string
    input_str[ch_number - 1] = '\0';
    ch_number--;

    // get permutation "alphabet"
    if (get_alphabet(ch_number, input_str, queue)) {
        printf("bad input");
        return 0;
    }
    get_permutations(queue, ch_number, permutation_str, &permutation_count, input_str, &was_comparing);
    unset_input(input);

    return 0;
}
