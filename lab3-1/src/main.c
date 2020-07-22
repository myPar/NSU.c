#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "sort.h"

int get_int(int *val, FILE *input) {
    if (!fscanf(input, "%d", val)) {
        printf("can't write data\n");
        unset_input(input);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *input;
    if ((input = set_input(argc, argv)) == NULL) {return 1;}

    int array_size;
    if (get_int(&array_size, input)) {return 1;}

    int *input_array = (int*) malloc(sizeof(int) * array_size);

    for (int i = 0; i < array_size; i++) {
        if (get_int(&input_array[i], input)) {
            free(input_array);
            return 1;
        }
    }
    heap_sort(input_array, array_size);

    for (int i = array_size - 1; i > -1; i--) {
        printf("%d ", input_array[i]);
    }
    free(input_array);
    unset_input(input);

    return 0;
}
