#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include <limits.h>
#include "structures.h"
#include "build_frame.h"

#define EXIT()                                          \
    free_adjacency_list(adjacency_list, vertex_number); \
    free(edges_number_array);                           \
    free(edges_list);                                   \
    exit(0);

void insert_edge_index(int **adjacency_list, int vertex_idx, int cur_idx, int *edges_number_array) {
    // increase number of output edges
    edges_number_array[vertex_idx]++;

    if (edges_number_array[vertex_idx] == 1) {
        // not need to reallocate memory, init adjacency list
        adjacency_list[vertex_idx][0] = cur_idx;
    }
    else {
        // memory reallocation
        adjacency_list[vertex_idx] = (int*) realloc(adjacency_list[vertex_idx], sizeof(int) * edges_number_array[vertex_idx]);
        // init adjacency list
        adjacency_list[vertex_idx][edges_number_array[vertex_idx] - 1] = cur_idx;
    }
}

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
    int *edges_number_array = (int*) malloc(sizeof(int) * vertex_number);
    Edge *edges_list = (Edge*) malloc (sizeof(Edge) * edge_number);
    int **adjacency_list = (int**) malloc(sizeof(int*) * vertex_number);

    for (int i = 0; i < vertex_number; i++) {
        edges_number_array[i] = 0;
        adjacency_list[i] = (int*) malloc(sizeof(int));
    }

    long start_vertex;
    long end_vertex;
    long edge_length;
    short cur_idx = 0;

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
        init_Edge((short) (start_vertex - 1), (short) (end_vertex - 1), edge_length, &edges_list[cur_idx]);
        int start_idx = start_vertex - 1;
        int end_idx = end_vertex - 1;

        insert_edge_index(adjacency_list, start_idx, cur_idx, edges_number_array);
        if (start_idx != end_idx) {
            insert_edge_index(adjacency_list, end_idx, cur_idx, edges_number_array);
        }
        cur_idx++;
    }
    bfs(adjacency_list, edges_list, edges_number_array,  vertex_number, edge_number);
    return 0;
}

#undef EXIT
