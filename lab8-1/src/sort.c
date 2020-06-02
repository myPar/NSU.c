#include "structures.h"

/// edges sorting methods, based on quick sort
int partion(Edge *edges_array, int left_idx, int right_idx) {
    int center_element = edges_array[(right_idx + left_idx)/2].size;
    int i = left_idx;
    int j = right_idx;

    while (i <= j) {
        // if left element less then center element increment left index
        while(edges_array[i].size < center_element) {
            i++;
        }
        // if right element more then center element decrement right index
        while(edges_array[j].size > center_element) {
            j--;
        }
        if (i >= j) {
            break;      // not need to swap any elements
        }
        Edge copy = edges_array[i];
        edges_array[i] = edges_array[j];
        edges_array[j] = copy;

        i++;
        j--;
    }
    return j;
}

void quick_sort(Edge *edges_array, int left_idx, int right_idx) {
    if (left_idx < right_idx) {
        int middle_idx = partion(edges_array, left_idx, right_idx);
        quick_sort(edges_array, left_idx, middle_idx);
        quick_sort(edges_array, middle_idx + 1, right_idx);
    }
    else {
        return;
    }
}
