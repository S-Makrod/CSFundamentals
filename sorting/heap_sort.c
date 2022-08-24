#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 5

void swap(int heap[], int a, int b) {
    // Swap the elements
    int temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

int get_min(int heap[], int parent, int n) {
    // if a left child exists
    if (parent*2+1 < n) {
        int child1 = parent*2+1;

        // if a right child exists return smaller one
        if (parent*2+2 < n) {
            int child2 = parent*2+2;
            return heap[child1] < heap[child2] ? child1 : child2;
        }

        return child1;
    }

    // no child
    return -1;
}

void heapify(int heap[], int start, int n) {
    int smaller_child = 1;
    int index = start;

    do {
        int parent = heap[index];
        // get smallest child of parent
        int smallest_child = get_min(heap, index, n);

        // if child exists and is smaller in priority swap them and update index
        if (smallest_child != -1 && parent > heap[smallest_child]) {
            swap(heap, index, smallest_child);
            index = smallest_child;
        // if child is bigger, element is in right spot so end loop
        } else {
            smaller_child = 0;
        }
    } while(smaller_child && index < n);

    return;
}

int heapExtractMin(int heap[], int *n) {
    if(*n < 1) return -1;
    int val = heap[0];

    // swap the last element in heap and current min
    swap(heap, 0, *n-1);
    // decrease numItems to 'discard' min
    *n = *n - 1;
    // heapify to fix min heap structure
    heapify(heap, 0, *n);

    // return val
    return val;
}

void heapSort(int arr[], int n) {
    int copy[n], size = n;
    int parent_index = ((n-1) % 2 == 0) ? (n-1)/2-1 : (n-1)/2;

    for(int i = 0; i < n; i++) copy[i] = arr[i];
    for(int i = parent_index; i >= 0; i--) heapify(copy, i, n);
    for(int i = 0; i < n; i++) arr[i] = heapExtractMin(copy, &size);
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    heapSort(array, ARRAY_LENGTH);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}