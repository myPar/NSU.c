#include <stdio.h>
#include <stdlib.h>
#include "input.h"

void swap(int *input_array, int idx1, int idx2) {
    int copy = input_array[idx1];
    input_array[idx1] = input_array[idx2];
    input_array[idx2] = copy;
}

void heap_sort(int *input_array, int array_size) {
    int last_child_idx = array_size - 1;
    int paren_idx;

    while(last_child_idx >= 0) {
        // after pop of the heaps root rebuild the heap [0..last_child_idx]
        for (paren_idx = (last_child_idx - 1) / 2; paren_idx > -1; paren_idx--) {
            // current parent node has two children
            int left_child_idx = paren_idx * 2 + 1;
            int right_child_idx = paren_idx * 2 + 2;

            if (right_child_idx <= last_child_idx) {
                int max_idx;
                // get index of child with max value
                if (input_array[left_child_idx] > input_array[right_child_idx]) {
                    max_idx = left_child_idx;
                }
                else {
                    max_idx = right_child_idx;
                }
                if (input_array[paren_idx] < input_array[max_idx]) {
                    // swap parent and child
                    swap(input_array, paren_idx, max_idx);
                }
            }
            else {
                // current parent node can have only one child
                if (left_child_idx <= last_child_idx) {
                    if (input_array[paren_idx] < input_array[left_child_idx]) {
                        // swap parent and child
                        swap(input_array, paren_idx, left_child_idx);
                    }
                }
            }
        }
        // pop root from heap
        swap(input_array, 0, last_child_idx);
        last_child_idx--;
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

    heap_sort(input_array, array_size);

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