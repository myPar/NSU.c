#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include <limits.h>
#include "structures.h"
#include "build_frame.h"

#define EXIT()                              \
    free_matrix(matrix, vertex_number);     \
    exit(0);

int main(int argc, char *argv[]) {
    FILE *input = set_input(argc, argv);
    long vertex_number;
    long edge_number;

    if (read_int(input, &vertex_number, "bad number of vertices",
            0, 5000)) {
        exit(0);
    }
    if (vertex_number == 0) {
        // there is no frame in graph without edges
        printf("no spanning tree");
        exit(0);
    }
    if (read_int(input, &edge_number, "bad number of edges",
            0, vertex_number * (vertex_number - 1) / 2)) {
        exit(0);
    }
    int **matrix = make_matrix(vertex_number);

    long start_vertex;
    long end_vertex;
    long edge_length;

    for (int i = 0; i < edge_number; i++) {
        if (read_int(input, &start_vertex, "bad vertex", 1, vertex_number)) {
            //free_matrix(matrix, vertex_number);
            EXIT()
        }
        if (read_int(input, &end_vertex, "bad vertex", 1, vertex_number)) {
            //free_matrix(matrix, vertex_number);
            EXIT()
        }
        if (read_int(input, &edge_length, "bad length", 0, INT_MAX)) {
            //free_matrix(matrix, vertex_number);
            EXIT()
        }
        int start_idx = start_vertex - 1;
        int end_idx = end_vertex - 1;

        matrix[start_idx][end_idx] = edge_length;
        matrix[end_idx][start_idx] = edge_length;
    }
    bfs(matrix,  vertex_number, edge_number);
    return 0;
}

#undef EXIT
