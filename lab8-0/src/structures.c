#include <stdlib.h>

/// matrix constructor, returns initialized by -1 for each element matrix
int **make_matrix(int size) {
    int **matrix = (int**) malloc(sizeof(int*) * size);

    for (int i = 0; i < size; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * size);
    }
    // init matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = -1;
        }
    }
    return matrix;
}

/// free dynamic memory from the matrix
void free_matrix(int **matrix, int size) {
    for (int i = 0 ; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
