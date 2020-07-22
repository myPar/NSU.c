#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "input.h"
#include "structures.h"
#include "way_find.h"

int main(int argc, char *argv[]) {
    FILE *input = set_input(argc, argv);
    long vertex_number;
    long edge_number;
    long path_start;
    long path_end;
    // read vertices number
    if (read_int(input, &vertex_number, "bad number of vertices",
                 0, 5000, 0)) {
        exit(0);
    }
    // read start path vertex
    if (read_int(input, &path_start, "bad vertex",
                 1, vertex_number, 0)) {
        exit(0);
    }
    // read end path vertex
    if (read_int(input, &path_end, "bad vertex",
                 1, vertex_number, 0)) {
        exit(0);
    }
    // read edges number
    if (read_int(input, &edge_number, "bad number of edges",
                 0, vertex_number * (vertex_number - 1) / 2, 0)) {
        exit(0);
    }
    int **matrix = make_matrix(vertex_number);
    long start_vertex;
    long end_vertex;
    long edge_length;

    for (int i = 0 ; i < edge_number; i++) {
        if (read_int(input, &start_vertex, "bad vertex", 1, vertex_number, 0)) {
            free_matrix(matrix, vertex_number);
            exit(0);
        }
        if (read_int(input, &end_vertex, "bad vertex", 1, vertex_number, 0)) {
            free_matrix(matrix, vertex_number);
            exit(0);
        }
        int result;
        if (i == edge_number - 1) {
            result = read_int(input, &edge_length, "bad length", 0, INT_MAX, 1);
        }
        else {
            result = read_int(input, &edge_length, "bad length", 0, INT_MAX, 0);
        }
        if (result) {
            free_matrix(matrix, vertex_number);
            exit(0);
        }
        matrix[start_vertex - 1][end_vertex - 1] = (int) edge_length;
        matrix[end_vertex - 1][start_vertex - 1] = (int) edge_length;
    }
    find_path(matrix, (short) vertex_number, (short) path_start, (short) path_end);

    return 0;
}
