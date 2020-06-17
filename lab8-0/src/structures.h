#ifndef PRIM_STRUCTURES_H
#define PRIM_STRUCTURES_H
typedef struct GraphEdge Edge;
struct GraphEdge {
    short start_vertex;
    short end_vertex;
    int size;
};

typedef struct PriorityQueue Heap;
struct PriorityQueue {
    Edge **array;
    int size;
};

int **make_matrix(int size);
void free_matrix(int **matrix, int size);

Edge *make_Edge(short start_vertex, short end_vertex, int length);

Heap *make_heap(int memory_size);
void free_heap(Heap *heap);
void insert_edge(Edge *edge, Heap *heap);
Edge *pop_edge(Heap *heap);
#endif
