#include <stdio.h>
#include "input.h"
#include "change_base.h"

int main() {
    int val1 = 0;
    int val2 = 0;
    char *input_number;

    if (get_input(&val1, &val2, &input_number) == -1) {
        printf("%s", "bad input");
        exit(0);
    }
    else {
        double input_number_10 = get_base10_number(input_number, val1);
        char str[1024];
        new_base_convert(str, input_number_10, val2);
        printf("%s", str);
    }
    // input_number have been initialized by pointer on dynamic memory
    free(input_number);
    return 0;
}

