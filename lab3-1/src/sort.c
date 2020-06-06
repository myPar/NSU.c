void swap(int *input_array, int idx1, int idx2) {
    int copy = input_array[idx1];
    input_array[idx1] = input_array[idx2];
    input_array[idx2] = copy;
}

void sifting_up(int *heap, int array_size) {
    int cur_idx = array_size - 1;

    while (cur_idx > 0) {
        if (heap[cur_idx] < heap[cur_idx / 2]) {
            swap(heap, cur_idx, cur_idx / 2);
            cur_idx /= 2;
        }
        else {
            break;
        }
    }
}

void sifting_down(int *heap, int array_size) {
    int cur_idx = 0;

    while (cur_idx < array_size / 2) {
        int left_child_idx = cur_idx * 2 + 1;
        int right_child_idx = cur_idx * 2 + 2;
        int min_idx;

        if (right_child_idx > array_size - 1) {
            min_idx = left_child_idx;
        }
        else {
            min_idx = heap[left_child_idx] < heap[right_child_idx] ? left_child_idx : right_child_idx;
        }
        if (heap[cur_idx] > heap[min_idx]) {
            swap(heap, min_idx, cur_idx);
            cur_idx = min_idx;
        }
        else {
            break;
        }
    }
}

void make_heap(int *input_array, int array_size) {
    // sift up all elements from 1 to array_size indices
    for (int i = 1; i < array_size; i++) {
        sifting_up(input_array, i + 1);
    }
}

void heap_sort(int *input_array, int array_size) {
    make_heap(input_array, array_size);
    int cur_size = array_size;

    while (cur_size > 1) {
        swap(input_array, 0, cur_size - 1);
        cur_size--;
        sifting_down(input_array, cur_size);
    }
}
