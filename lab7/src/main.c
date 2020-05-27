#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define EXIT()                                      \
    free(colour_array);                             \
    free(edges_number_array);                       \
    for (int l = 0; l < vertices_number; l++) {     \
        free(adjacency_list[l]);                    \
    }                                               \
    free(adjacency_list);                           \
    exit(0);

void dfs(char *colour_array, short *edges_number_array, short **adjacency_list,
        int cur_idx, int vertices_number, short *result, int *res_idx) {
    // mark as 'grey' node
    colour_array[cur_idx] = 1;
    int edges_number = edges_number_array[cur_idx];
    // loop on adjacency list of current node
    for (int i = 0; i < edges_number; i++) {
        short cur_node = adjacency_list[cur_idx][edges_number - i - 1];

        if (colour_array[cur_node] == 0) {
            dfs(colour_array, edges_number_array, adjacency_list, cur_node, vertices_number,  result, res_idx);
        }
        else {
            if (colour_array[cur_node] == 1) {
                // there is a loop in the graph
                printf("impossible to sort");
                free(result);
                EXIT()
            }
        }
    }
    // mark as black
    colour_array[cur_idx] = 2;
    result[*res_idx] = (short) cur_idx + 1;
    (*res_idx)++;
}

int main(int argc, char *argv[]) {
    FILE *input = set_input(argc, argv);
    int vertices_number = 0;
    int edges_number = 0;

    // read vertices number
    if (read_int(input, &vertices_number, "bad number of vertices",
                 0, 1000)) {
        exit(0);
    }
    // read edges number
    if (read_int(input, &edges_number, "bad number of edges",
                 0, vertices_number * (vertices_number - 1) / 2)) {
        exit(0);
    }
    char *colour_array = (char*) malloc(sizeof(char)*vertices_number);
    short *edges_number_array = (short*) malloc(sizeof(short) * vertices_number);
    short **adjacency_list = (short**) malloc(vertices_number * (sizeof(short*)));

    // initial initialisation
    for (int i = 0; i < vertices_number; i++) {
        colour_array[i] = 0;
        edges_number_array[i] = 0;
        // allocate memory for one element
        adjacency_list[i] = (short*) malloc(sizeof(short));
    }
    int start_node;
    int end_node;

    // reading edges:
    for (int i = 0; i < edges_number; i++) {
        // read start node
        if (read_int(input, &start_node, "bad vertex",
                     0, vertices_number)) {
            EXIT()
        }
        // read end node
        if (read_int(input, &end_node, "bad vertex",
                     0, vertices_number)) {
            EXIT()
        }
        int idx = start_node - 1;
        edges_number_array[idx]++;

        if (edges_number_array[idx] == 1) {
            // initial assignment
            adjacency_list[idx][0] = (short) end_node - 1;
        }
        else {
            // memory reallocation and assignment
            adjacency_list[idx] = (short*) realloc(adjacency_list[idx], sizeof(short) * edges_number_array[idx]);
            adjacency_list[idx][edges_number_array[idx] - 1] = (short) end_node - 1;
        }
    }
    short *answer = (short*) malloc(sizeof(short) * vertices_number);
    int answer_idx = 0;
    int *pointer = &answer_idx;

    for (int i = 0; i < vertices_number; i++) {
        if (colour_array[i] == 0) {
            dfs(colour_array, edges_number_array, adjacency_list, i, vertices_number, answer, pointer);
        }
    }
    for (int i = vertices_number - 1 ; i > -1; i--) {
        printf("%hi ", answer[i]);
    }

    free(answer);
    EXIT()
}
#undef EXIT