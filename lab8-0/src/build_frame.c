#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "structures.h"

int pop_min(unsigned int *distance_array, int *marked_array, int vertex_number) {
    unsigned int min = (unsigned int) INT_MAX + 1;
    int vertex = -1;

    for (int i = 0; i < vertex_number; i++) {
        if (distance_array[i] < min && !marked_array[i]) {
            min = distance_array[i];
            vertex = i;
        }
    }
    return vertex;
}

#define EXIT()                             \
    free_matrix(matrix, vertex_number);    \
    free(distance_array);                  \
    free(parent_array);                    \
    free(marked_array);                    \
    free(answer);                          \
    exit(0);

void bfs(int **matrix, int vertex_number) {
    unsigned int *distance_array = (unsigned int*) malloc(sizeof(int) * vertex_number);
    int *parent_array = (int*) malloc(sizeof(int) * vertex_number);
    int *marked_array = (int*) malloc(sizeof(int) * vertex_number);
    Edge *answer = (Edge*) malloc(sizeof(Edge) * (vertex_number - 1));

    int marked_count = 0;

    for (int i = 0; i < vertex_number; i++) {
        distance_array[i] = (unsigned int) INT_MAX + 1;
        parent_array[i] = -1;
        marked_array[i] = 0;
    }
    distance_array[0] = 0;
    int cur_vertex = 0;

    while (marked_count < vertex_number - 1) {

        for (int i = 0; i < vertex_number; i++) {
            int weight = matrix[cur_vertex][i];

            if (weight != -1) {
                if (!marked_array[i] && (unsigned int) weight < distance_array[i]) {
                    distance_array[i] = weight;
                    parent_array[i] = cur_vertex;
                }
            }
        }
        marked_array[cur_vertex] = 1;

        if ((cur_vertex = pop_min(distance_array, marked_array, vertex_number)) == -1) {
            printf("no spanning tree");
            EXIT()
        }
        answer[marked_count].end_vertex = (short) (cur_vertex + 1);
        answer[marked_count].start_vertex = (short) (parent_array[cur_vertex] + 1);
        marked_count++;
    }
    for (int i = 0; i < vertex_number - 1; i++) {
        printf("%d %d\n", answer[i].start_vertex, answer[i].end_vertex);
    }
    EXIT()
}
