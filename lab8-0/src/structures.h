#ifndef PRIM_STRUCTURES_H
#define PRIM_STRUCTURES_H
typedef struct GraphEdge Edge;
struct GraphEdge {
    short start_vertex;
    short end_vertex;
};

typedef struct PriorityQueue Heap;
struct PriorityQueue {
    int *array;
    int size;
};

int **make_matrix(int size);
void free_matrix(int **matrix, int size);
#endif
