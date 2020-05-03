#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
# include <assert.h>
#include <ctype.h>

bool is_correct_number(int number_base, char number[]) {
    char *base_array = "0123456789abcdef";
    bool is_correct;
    bool was_point = false;

    for (int i = 0; i < strlen(number); i++) {
        is_correct = false;

        for (int j = 0; j < number_base; j++) {
            if (number[i] == base_array[j]) {
                is_correct = true;
                break;
            }
            if (number[i] == '.') {
                // check consistence of second '.' in number
                if (was_point) {
                    return false;
                }
                was_point = true;
                is_correct = true;
                break;
            }
        }
        if (!is_correct) {
            return false;
        }
    }
    return true;
}

int get_input(int *arg1_pointer, int *arg2_pointer, char** arg3_pointer) {
    int start_base = 0, end_base = 0;
    char *input_number = (char*) malloc(sizeof(char)*14);

    scanf("%d%d", &start_base, &end_base);

    if (!(start_base >= 2 && start_base <= 16) || !(end_base >= 2  && end_base <= 16)) {
        return -1;
    }

    scanf("%s", input_number);
    // set all characters to lowercase
    for (int i = 0; i < strlen(input_number); i++) {
        input_number[i] = tolower(input_number[i]);
    }
    if (!is_correct_number(start_base, input_number)) {
        return -1;
    }

    *arg1_pointer = start_base;
    *arg2_pointer = end_base;
    *arg3_pointer = input_number;
    return 0;
}

