#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "structures.h"
#include "sort.h"

/// make graph frame
void make_frame(Edge *edge_array, int edges_number, short vertex_number) {
    short *set_array = (short*) malloc(sizeof(short) * vertex_number);
    short *rank_array = (short*) malloc(sizeof(short) * vertex_number);
    Edge **answer = (Edge**) malloc(sizeof(Edge*) * edges_number);
    int cur_idx = 0;

    for (int i = 0 ; i < vertex_number; i++) {
        //marked_array[i] = 0;
        rank_array[i] = 1;
    }
    // make set for each vertex
    for (short i = 0 ; i < vertex_number; i++) {
        make_set(i, set_array);
    }
    // start number of sets is equal number of vertex
    int sets_count = vertex_number;
    // sort edges
    quick_sort(edge_array, 0, edges_number - 1);

    for (int i = 0; i < edges_number; i++) {
        Edge *cur_edge = &edge_array[i];

        if (find(cur_edge->start_vertex, set_array) != find(cur_edge->end_vertex, set_array)) {
            unite(cur_edge->start_vertex, cur_edge->end_vertex, set_array, rank_array);
            sets_count--;
            answer[cur_idx] = cur_edge;
            cur_idx++;
        }
    }
    assert(sets_count > 0);

    if (sets_count == 1) {
        for (int i = 0; i < cur_idx; i++) {
            printf("%d %d\n", answer[i]->start_vertex + 1, answer[i]->end_vertex + 1);
        }
    }
    else {
        printf("no spanning tree");
    }
    free(set_array);
    free(rank_array);
    free(answer);
    free(edge_array);
}
