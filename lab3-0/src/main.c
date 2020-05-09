#include <stdio.h>
#include "input.h"
#include <stdlib.h>

void swap(int *input_array, int idx1, int idx2) {
    int copy = input_array[idx1];
    input_array[idx1] = input_array[idx2];
    input_array[idx2] = copy;
}

int partion(int *input_array, int left_idx, int right_idx) {
    int center_element = input_array[(right_idx + left_idx)/2];
    int i = left_idx;
    int j = right_idx;

    while (i <= j) {
        // if left element less then center element increment left index
        while(input_array[i] < center_element) {
            i++;
        }
        // if right element more then center element decrement right index
        while(input_array[j] > center_element) {
            j--;
        }
        if (i >= j) {
            break;      // not need to swap any elements
        }
        swap(input_array, i, j);
        i++;
        j--;
    }
    return j;
}

void quick_sort(int *input_array, int left_idx, int right_idx) {
    if (left_idx < right_idx) {
        int middle_idx = partion(input_array, left_idx, right_idx);
        quick_sort(input_array, left_idx, middle_idx);
        quick_sort(input_array, middle_idx + 1, right_idx);
    }
    else {
        return;
    }
}

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