#include <stdlib.h>
#include <assert.h>
#include "structures.h"

/// matrix constructor, returns initialized by -1 for each element matrix
int **make_matrix(int size) {
    int **matrix = (int**) malloc(sizeof(int*) * size);

    for (int i = 0; i < size; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * size);
    }
    // init matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = -1;
        }
    }
    return matrix;
}

/// free dynamic memory from the matrix
void free_matrix(int **matrix, int size) {
    for (int i = 0 ; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// edge initialization
Edge *make_Edge(short start_vertex, short end_vertex, int length) {
    Edge *edge = (Edge*) malloc(sizeof(Edge));
    edge->start_vertex = start_vertex;
    edge->end_vertex = end_vertex;
    edge->size = length;

    return edge;
}
// heap constructor
Heap *make_heap(int size) {
    Heap *heap = (Heap*) malloc(sizeof(Heap));
    assert(heap != NULL);
    heap->array = (Edge**) malloc(sizeof(Edge*) * size);
    assert(heap->array != NULL);
    heap->size = 0;

    return heap;
}
// free heap memory
void free_heap(Heap *heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->array[i]);
    }
    free(heap->array);
    free(heap);
}
// heap methods:
void swap(Heap *heap, int idx1, int idx2) {
    Edge *edge = heap->array[idx1];
    heap->array[idx1] = heap->array[idx2];
    heap->array[idx2] = edge;
}

void sifting_up(Heap *heap) {
    int cur_idx = heap->size - 1;

    while (cur_idx > 0) {
        int paren_idx = cur_idx / 2;

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
    int cur_idx = 0;
    // while element has child
    while (cur_idx < heap->size / 2) {
        int min_idx;
        int child1_idx = cur_idx * 2 + 1;
        int child2_idx = cur_idx * 2 + 2;
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
