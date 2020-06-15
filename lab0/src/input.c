#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
enum{max_size = 20};

bool is_correct_number(int number_base, char number[]) {
    char *base_array = "0123456789abcdef";
    bool was_point = false;
    char cut_base_array[max_size];

    strncpy(cut_base_array, base_array, number_base);
    cut_base_array[number_base] = '\0';

    for (int i = 0; i < (int) strlen(number); i++) {
        bool is_correct = false;

        if (strchr(cut_base_array, number[i]) != NULL) {
            is_correct = true;
        }
        if (number[i] == '.') {
            // check consistence of second '.' in number
            if (was_point) {
                return false;
            }
            was_point = true;
            is_correct = true;
        }
        if (!is_correct) {
            return false;
        }
    }
    return true;
}

int get_input(int *arg1_pointer, int *arg2_pointer, char* arg3_pointer, char *format) {
    int start_base = 0, end_base = 0;

    if (!scanf("%d%d", &start_base, &end_base)) {
        printf("can't read integer values\n");
        return 1;
    }

    if (!(start_base >= 2 && start_base <= 16) || !(end_base >= 2  && end_base <= 16)) {
        return -1;
    }

    if (!scanf(format, arg3_pointer)) {
        printf("can't read a string value\n");
        return 1;
    }
    // set all characters to lowercase
    for (int i = 0; i < (int) strlen(arg3_pointer); i++) {
        arg3_pointer[i] = (char) tolower(arg3_pointer[i]);
    }
    if (!is_correct_number(start_base, arg3_pointer)) {
        return -1;
    }
    *arg1_pointer = start_base;
    *arg2_pointer = end_base;

    return 0;
}
