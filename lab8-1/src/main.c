#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "input.h"
#include "structures.h"
#include "make_frame.h"

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
    Edge *edge_array = (Edge*) malloc(sizeof(Edge) * edge_number);

    long start_vertex;
    long end_vertex;
    long edge_length;

    for (int i = 0 ; i < edge_number; i++) {
        if (read_int(input, &start_vertex, "bad vertex", 1, vertex_number)) {
            free(edge_array);
            exit(0);
        }
        if (read_int(input, &end_vertex, "bad vertex", 1, vertex_number)) {
            free(edge_array);
            exit(0);
        }
        if (read_int(input, &edge_length, "bad length", 0, INT_MAX)) {
            free(edge_array);
            exit(0);
        }
        init_edge(&edge_array[i], (short) (start_vertex - 1), (short) (end_vertex - 1), (int) edge_length);
    }
    make_frame(edge_array, edge_number, (short) vertex_number);
    return 0;
}
