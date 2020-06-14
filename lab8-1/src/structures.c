#include "structures.h"
/// DSU implementation

/// making new set with one element
void make_set(short x, short *set_array) {
    // element become a main element of the set
    set_array[x] = x;
}
/// returns root of the node's set
short find(short x, short *set_array) {
    if (x == set_array[x]) {
        // return root
        return x;
    }
    // reassignment of the parent for all nodes on the way
    return set_array[x] = find(set_array[x], set_array);
}
/// unite sets with present nodes
void unite(short x, short y, short *set_array, short *rank_array) {
    // find roots of nodes sets
    x = find(x, set_array);
    y = find(y, set_array);

    if (rank_array[x] < rank_array[y]) {
        // unite trees by changing root
        set_array[x] = y;
    }
    else {
        set_array[y] = x;

        if (rank_array[x] == rank_array[y]) {
            // increment rank of new tree
            rank_array[x]++;
        }
    }
}
/// Edge initialization
void init_edge(Edge *edge, short start_vertex, short end_vertex, int size) {
    edge->start_vertex = start_vertex;
    edge->end_vertex = end_vertex;
    edge->size = size;
}
