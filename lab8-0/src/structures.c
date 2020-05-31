#include <stdlib.h>
#include "structures.h"
// edge initialization
void init_Edge(short start_vertex, short end_vertex, int length, Edge *edge) {
    edge->start_vertex = start_vertex;
    edge->end_vertex = end_vertex;
    edge->size = length;
}
void swap_edge(Edge *edge) {
    short copy = edge->start_vertex;
    edge->start_vertex = edge->end_vertex;
    edge->end_vertex = copy;
}
void free_adjacency_list(int **adjacency_list, int size) {
    for (int i = 0; i < size; i++) {
        free(adjacency_list[i]);
    }
    free(adjacency_list);
}

// heap constructor
Heap *make_heap(int memory_size) {
    Heap *heap = (Heap*) malloc(sizeof(Heap));
    heap->array = (Edge**) malloc(sizeof(Edge*) * memory_size);
    heap->size = 0;
    return heap;
}
// free heap memory
void free_heap(Heap *heap) {
    free(heap->array);
    free(heap);
}
// heap methods:
void swap(Heap *heap, size_t idx1, size_t idx2) {
    Edge *edge = heap->array[idx1];
    heap->array[idx1] = heap->array[idx2];
    heap->array[idx2] = edge;
}

void sifting_up(Heap *heap) {
    size_t cur_idx = heap->size - 1;
    size_t paren_idx;

    while (cur_idx > 0) {
        paren_idx = cur_idx / 2;

        if (heap->array[paren_idx]->size > heap->array[cur_idx]->size) {
            // swap
            swap(heap, paren_idx, cur_idx);
            cur_idx = paren_idx;
        }
        else {
            break;
        }
    }
}
// dipping root down to the heap
void sifting_down(Heap *heap) {
    size_t cur_idx = 0;
    size_t min_idx;
    size_t child1_idx;
    size_t child2_idx;
    // while element has child
    while (cur_idx < heap->size / 2) {
        child1_idx = cur_idx * 2 + 1;
        child2_idx = cur_idx * 2 + 2;
        // get index of minimum child
        if (child2_idx < heap->size && heap->array[child2_idx]->size < heap->array[child1_idx]->size) {
            min_idx = child2_idx;
        }
        else {
            min_idx = child1_idx;
        }
        if (heap->array[min_idx]->size < heap->array[cur_idx]->size) {
            swap(heap, min_idx, cur_idx);
            cur_idx = min_idx;
        }
        else {
            break;
        }
    }
}

// inserts new edge in the heap
void insert_edge(Edge *edge, Heap *heap) {
    heap->array[heap->size] = edge;
    heap->size++;
    sifting_up(heap);
}

// pop root from the heap
Edge *pop_edge(Heap *heap) {
    Edge *root = heap->array[0];
    // move last edge to the root
    heap->array[0] = heap->array[heap->size - 1];
    heap->array[heap->size - 1] = NULL;
    heap->size--;
    // drown root in the heap
    if (heap->size > 1) {
        sifting_down(heap);
    }
    // return removed element
    return root;
}