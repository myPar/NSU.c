#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

/// restore path method
short *restore_path(short *restore_array, short *path_length, short start, short end) {
    short size = 1;
    short *path = (short*) malloc(sizeof(short));
    path[0] = end;
    short cur_vertex = end;

    while (cur_vertex != start) {
        // move to next vertex in the path
        cur_vertex = restore_array[cur_vertex];
        // add new vertex in the path
        short *result = (short*) realloc(path, sizeof(short) * (size + 1));

        if (result == NULL) {
            free(path);
            exit(1);
        }
        else {
            path = result;
        }
        path[size] = cur_vertex;
        size++;
    }
    *path_length = size;    // remember path length

    return path;
}
#define EXIT()                          \
    free(restore_array);                \
    free(weight_array);                 \
    free(marked);                       \
    free_matrix(matrix, vertex_number); \
    exit(0);

/// main algorithm
void find_path(int **matrix, short vertex_number, short start_vertex, short end_vertex) {
    short *restore_array = (short*) malloc(sizeof(short) * vertex_number);
    unsigned int *weight_array = (unsigned int*) malloc(sizeof(int) * vertex_number);
    char *marked = (char*) malloc(sizeof(char) * vertex_number);

    int start_idx = start_vertex - 1;
    int end_idx = end_vertex - 1;

    // start paths initialization
    for (int i = 0; i < vertex_number; i++) {
        weight_array[i] = UINT_MAX;
        marked[i] = 0;
        restore_array[i] = -1;
    }
    weight_array[start_idx] = 0;
    restore_array[start_idx] = 0;

    while(1) {
        unsigned int min_weight = UINT_MAX;
        short min_vertex_idx = -1;

        // get vertex idx with min weight
        for (short j = 0; j < vertex_number; j++) {
            if (!marked[j] && weight_array[j] < min_weight) {
                min_weight = weight_array[j];
                min_vertex_idx = j;
            }
        }
        if (min_vertex_idx == -1) {
            // all vertices are marked or no way to the remaining vertices
            break;
        }
        // set new weights to the adjacent vertices
        for (int  i = 0; i < vertex_number; i++) {
            int edge_len = matrix[min_vertex_idx][i];

            if (edge_len != -1) {
                unsigned long length = min_weight + edge_len;
                // if new weight is less then previous
                if (length < weight_array[i]) {
                    if (length <= INT_MAX) {
                        weight_array[i] = (int) length;
                    }
                    else {
                        // the path limit is overflow
                        weight_array[i] = (unsigned int) INT_MAX + 1;
                    }
                    // write new parent
                    restore_array[i] = min_vertex_idx;
                }
            }
        }
        // mark processed vertex
        marked[min_vertex_idx] = 1;
    }
    // print paths
    for (int i = 0; i < vertex_number; i++) {
        switch (weight_array[i]) {
            case UINT_MAX:
                printf("oo ");
                break;
            case (unsigned int) INT_MAX + 1:
                printf("INT_MAX+ ");
                break;
            default:
                printf("%u ", weight_array[i]);
        }
    }
    printf("\n");
    // print path between start vertex and end vertex
    if (weight_array[end_idx] == UINT_MAX) {
        // no path to the end vertex
        printf("no path");
        EXIT()
    }
    else {
        // check overflow
        // if end vertex is equal start vertex it doesn't matter how much
        // adjacent vertices with weight > INT_MAX are there
        if (end_vertex != start_vertex) {
            short count = 0;

            for (int i = 0; i < vertex_number; i++) {
                if (matrix[end_idx][i] != -1) {
                    // summary weight <= 2 * INT_MAX + 1, so unsigned long type will be enough
                    unsigned long weigh = weight_array[i] + matrix[end_idx][i];

                    if (weigh > INT_MAX) {
                        count++;
                    }
                    if (count >= 2) {
                        printf("overflow");
                        EXIT()
                    }
                }
            }
        }
        // there is no overflow so print path vertices
        short path_length;
        short *path = restore_path(restore_array, &path_length, (short) start_idx, (short) end_idx);

        for (int i = 0; i < path_length; i++) {
            printf("%d ", path[i] + 1);
        }
        free(path);
        EXIT()
    }
}

#undef EXIT
