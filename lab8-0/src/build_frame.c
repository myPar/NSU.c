#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

#define EXIT()                                              \
    free_heap(queue);                                       \
    free_matrix(matrix, vertex_number);                     \
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
        free(edge);
    }
    return edge;
}
enum {max_vertex_number = 5000};

void bfs(int **matrix, int vertex_number, int edge_number) {
    // each frame vertex has NEW unused end vertex so
    // the frame size <= vertices number
    Edge *answer[max_vertex_number];
    char marked_array[max_vertex_number];

    Heap *queue = make_heap(edge_number);
    short marked_count = 1; // start vertex is already marked

    short cur_vertex = 0;
    int cur_idx = -1;

    for (int i = 0; i < vertex_number; i++) {
        marked_array[i] = 0;
    }
    marked_array[0] = 1;

    while (marked_count < vertex_number) {

        for (int i = 0; i < vertex_number; i++) {
            int length = matrix[cur_vertex][i];

            if (length != -1) {
                if (!marked_array[i]) {
                    Edge *edge = make_Edge(cur_vertex, (short) i, length);
                    // if vertex isn't in the frame
                    insert_edge(edge, queue);
                }
            }
        }
        cur_idx++;
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
        free(answer[i]);
    }
    EXIT()
}

#undef EXIT
