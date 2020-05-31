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
    size_t size;
};

void init_Edge(short start_vertex, short end_vertex, int length, Edge *edge);
void swap_edge(Edge *edge);
void free_adjacency_list(int **adjacency_list, int size);

Heap *make_heap(int memory_size);
void free_heap(Heap *heap);
void insert_edge(Edge *edge, Heap *heap);
Edge *pop_edge(Heap *heap);
#endif
