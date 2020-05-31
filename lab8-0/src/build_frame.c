#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

#define EXIT()                                              \
    free_adjacency_list(adjacency_list, vertex_number);     \
    free(marked_array);                                     \
    free(answer);                                           \
    free_heap(queue);                                       \
    free(edges_list);                                       \
    free(edges_number_array);                               \
    exit(0);

Edge *get_min_edge(Heap *heap, char *marked_array) {
    Edge *edge = NULL;
    // while heap not empty
    while (heap->size > 0) {
        edge = pop_edge(heap);
        // check edge for loop in graph
        if (!marked_array[edge->end_vertex]) {
            break;
        }
    }
    return edge;
}

void bfs(int **adjacency_list, Edge *edges_list, int *edges_number_array, int vertex_number, int edge_number) {
    Edge **answer = (Edge**) malloc(sizeof(Edge*));
    char *marked_array = (char*) malloc(sizeof(char) * vertex_number);

    Heap *queue = make_heap(edge_number);
    short marked_count = 1; // start vertex is already marked

    short cur_vertex = 0;
    int cur_idx = -1;

    for (int i = 0; i < vertex_number; i++) {
        marked_array[i] = 0;
    }
    marked_array[0] = 1;

    while (marked_count < vertex_number) {
        // add adjacent edges to the queue
        Edge *edge;
        // add new adjacent to the frame edges
        for (int i = 0; i < edges_number_array[cur_vertex]; i++) {
            edge = &edges_list[adjacency_list[cur_vertex][i]];
            if (edge->start_vertex != cur_vertex && !marked_array[edge->start_vertex]) {
                swap_edge(edge);
            }
            if (!marked_array[edge->end_vertex]) {
                // if vertex isn't in the frame
                insert_edge(edge, queue);
            }
        }
        cur_idx++;
        answer = (Edge**) realloc(answer, sizeof(Edge*) * (cur_idx + 1));
        // add not included to the frame edge
        answer[cur_idx] = get_min_edge(queue, marked_array);

        if (answer[cur_idx] == NULL) {
            printf("no spanning tree");
            EXIT()
        }
        // change current vertex
        cur_vertex = answer[cur_idx]->end_vertex;
        marked_count++;
        marked_array[cur_vertex] = 1;
    }
    // print frame
    for (int i = 0; i < cur_idx + 1; i++) {
        printf("%d %d\n", answer[i]->start_vertex + 1, answer[i]->end_vertex + 1);
    }
    EXIT()
}

#undef EXIT
