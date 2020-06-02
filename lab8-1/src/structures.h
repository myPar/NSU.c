#ifndef KRASKAL_STRUCTURES_H
#define KRASKAL_STRUCTURES_H
typedef struct GraphEdge Edge;
struct GraphEdge{
    short start_vertex;
    short end_vertex;
    int size;
};
void  init_edge(Edge *edge, short start_vertex, short end_vertex, int size);

void make_set(short x, short *set_array);
short find(short x, short *set_array);
void unite(short x, short y, short *set_array, short *rank_array);
#endif
