#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "change_base.h"

enum{size = 1024, digit_number = 13};
static char* format = "%13s";

int main() {
    int val1 = 0;
    int val2 = 0;
    char *input_number = (char*) malloc(sizeof(char) * (digit_number + 1));
    int result = get_input(&val1, &val2, input_number, format);

    if (result == -1) {
        printf("%s", "bad input");
        result = 0;
    }
    else if (result == 0){
        double input_number_10;

        if (get_base10_number(input_number, val1, &input_number_10) != NULL) {
            char str[size];

            new_base_convert(str, input_number_10, val2, digit_number);
            printf("%s", str);
        }
    }
    // input_number have been initialized by pointer on dynamic memory
    free(input_number);
    return result;
}
