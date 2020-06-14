#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "change_base.h"

int main() {
    int val1 = 0;
    int val2 = 0;
    char *input_number;
    int result = get_input(&val1, &val2, &input_number);

    if (result == 1) {
        return 1;
    }
    if (result == -1) {
        printf("%s", "bad input");
        return 0;
    }
    else {
        double input_number_10;

        if (get_base10_number(input_number, val1, &input_number_10) == NULL) {
            return 0;
        }
#define size 1024
        char str[size];
#undef size
        new_base_convert(str, input_number_10, val2);
        printf("%s", str);
    }
    // input_number have been initialized by pointer on dynamic memory
    free(input_number);
    return 0;
}
