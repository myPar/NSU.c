#include <stdio.h>
#include <stdlib.h>
#include "input.h"

typedef struct GraphEdge Edge;
struct GraphEdge {
    int idx;   // node index
    Edge *next;         // pointer to next edge of current node
};
Edge *make_edge(int idx) {
    Edge *edge = (Edge*) malloc(sizeof(Edge));
    edge->idx = idx;
    edge->next = NULL;
    return edge;
}
// init next edge in current node
void init_edge(Edge *head, int node_idx) {
    if (head->idx == -1) {
        // init first edge in the node
        head->idx = node_idx;
    }
    else {
        // init lats edge
        Edge *cur_edge = head;

        while (cur_edge->next != NULL) {
            cur_edge = cur_edge->next;
        }
        cur_edge->next = make_edge(node_idx);
    }
}
// free linked list memory
void free_list(Edge *head) {
    if (head->next == NULL) {
        free(head);
    }
    else {
        Edge *next_pointer = head->next;
        free(head);
        free_list(next_pointer);
    }
}
void free_edge_list(Edge *edge_list, int list_size) {
    Edge *cur_edge;
    for (int i = 0; i < list_size; i++) {
        cur_edge = &edge_list[i];
        if (cur_edge->next != NULL) {
            free_list(cur_edge->next);
        }
    }
    free(edge_list);
}

void top_sort(Edge *edge_list, int *edge_number_list, int vertex_number) {
    int vertex_idx;
    int cur_idx = 0;
    int *output_list = (int*) malloc(sizeof(int) * vertex_number);

    while (cur_idx < vertex_number) {
        vertex_idx = -1;

        for (int i = 0; i < vertex_number; i++) {
            if (edge_number_list[i] == 0) {
                // mark vertex as traversed
                edge_number_list[i] = -1;
                vertex_idx = i;
                break;
            }
        }
        if (vertex_idx == -1) {
            printf("impossible to sort");
            free_edge_list(edge_list, vertex_number);
            free(edge_number_list);
            exit(0);
        }
        // add vertex to output list
        output_list[cur_idx] = vertex_idx + 1;
        cur_idx++;
        Edge *cur_edge = &edge_list[vertex_idx];

        while (cur_edge != NULL) {
            // decrease input edges number for adjacent nodes
            if (cur_edge->idx > -1) {
                edge_number_list[cur_edge->idx]--;
            }
            cur_edge = cur_edge->next;
        }
    }
    for (int i = 0; i < vertex_number; i++) {
        printf("%d ", output_list[i]);
    }
    free_edge_list(edge_list, vertex_number);
    free(edge_number_list);
    free(output_list);
}

int main(int argc, char *argv[]) {
    FILE *input = set_input(argc, argv);
    int vertices_number = 0;
    int edges_number = 0;

    Edge *edge_list;
    int *edge_number_list;

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
    edge_list = (Edge*) malloc(sizeof(Edge) * vertices_number);
    edge_number_list = (int*) malloc(sizeof(int) * vertices_number);

    // start lists initialisation:
    for (int i = 0; i < vertices_number; i++) {
        edge_list[i] = *make_edge(-1);
    }
    for (int i = 0; i < vertices_number; i++) {
        edge_number_list[i] = 0;
    }
    int start_node;
    int end_node;

    // reading edges:
    for (int i = 0; i < edges_number; i++) {
        // read start node
        if (read_int(input, &start_node, "bad vertex",
                0, vertices_number)) {
            free_edge_list(edge_list, vertices_number);
            free(edge_number_list);
            exit(0);
        }
        // read end node
        if (read_int(input, &end_node, "bad vertex",
                     0, vertices_number)) {
            free_edge_list(edge_list, vertices_number);
            free(edge_number_list);
            exit(0);
        }
        init_edge(&edge_list[start_node - 1], end_node - 1);
        edge_number_list[end_node - 1]++;
    }
    top_sort(edge_list, edge_number_list, vertices_number);

    return 0;
}
