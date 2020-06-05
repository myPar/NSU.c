#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "sort.h"

#define GET_INT(x)                  \
    if (!fscanf(input, "%d", x)) {  \
        printf("can't write data"); \
        exit(1);                    \
    }

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    input = set_input(argc, argv);

    int array_size;
    GET_INT(&array_size)
    int *input_array = (int*) malloc(sizeof(int) * array_size);

    for (int i = 0; i < array_size; i++) {
        GET_INT(&(input_array[i]))
    }

    quick_sort(input_array, 0, array_size - 1);

    for (int i = 0; i < array_size; i++) {
        printf("%d%s", input_array[i], " ");
    }

    free(input_array);
    if (input != stdin) {
        fclose(input);
    }
    return 0;
}

#undef GET_INT