#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void get_permutations(int queue[10], int queue_size, char *perm_str, int *perm_count,
        char *input_perm, bool *was_comparing) {

    if (*perm_count <= 0) {
        exit(0);
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
        char str_number[10] = {0};

        for (int i = 0; i < 10; i++) {
            if (queue[i] != 0) {
                // make new copy for every iteration
                int queue_copy[10];
                memcpy(queue_copy, queue, sizeof(int) * 10);

                char perm_str_copy[11] = {0};
                strcpy(perm_str_copy, perm_str);

                // get i-number for current permutation
                queue_copy[i] = 0;
                // convert i to string for merging it
                // with current permutation string
                itoa(i, str_number, 10);
                strcat(perm_str_copy, str_number);
                get_permutations(queue_copy, queue_size - 1, perm_str_copy,
                                 perm_count, input_perm, was_comparing);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    char *input_file_name;
    FILE *input = NULL;

    char input_str[20] = {0};
    char count_str[1024] = {0};
    int queue[10] = {0};

    int permutation_count = 0;
    char permutation_str[11] = {0};

    bool was_comparing = false;

    if (argc > 1) {
        if (argc != 2) {
            printf("incorrect number of arguments: \nshould be input file name or no arguments");
            exit(1);
        }
        input_file_name = argv[1];

        if (!(input = fopen(input_file_name, "rt"))) {
            printf("can't open the file for reading");
            exit(1);
        }
    }
    else {
        input = stdin;
    }
    if (fgets(input_str, sizeof(input_str), input) == NULL) {
        printf("can't read the string\n");
        exit(1);
    }
    if (fgets(count_str, sizeof(count_str), input) == NULL) {
        printf("can't read the string\n");
        exit(1);
    }
    size_t ch_number = strlen(input_str);
    permutation_count = atoi(count_str);

    // cut '\n' in the end of the string
    input_str[ch_number - 1] = '\0';
    ch_number--;
    int cur_int;

    // get permutation "alphabet"
    for (int i = 0; i < ch_number; i++) {
        cur_int = input_str[i] - '0';

        if (cur_int > 9 || cur_int < 0) {
            // unexpected character in input string
            printf("bad input");
            exit(0);
        }
        queue[cur_int]++;

        if (queue[cur_int] > 1) {
            // input string is not the permutation
            printf("bad input");
            exit(0);
        }
    }
    get_permutations(queue, ch_number, permutation_str, &permutation_count, input_str, &was_comparing);

    if (input != stdin) {
        fclose(input);
    }
    return 0;
}
