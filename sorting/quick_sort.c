#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_LENGTH 5

void swap(int arr[], int a, int b) {
    // Swap the elements
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition(int arr[], int l, int h) {
    srand(time(0));
    swap(arr, (rand() % (h-l)) + l, h);
    int pivot = arr[h], i = l-1;

    for(int j = l; j < h; j++) {
        if(arr[j] <= pivot) {
            i++;
            swap(arr, i, j);
        }
    }

    swap(arr, i+1, h);
    return i+1;
}

void quickSort(int arr[], int l, int h) {
    if(l >= h) return;

    int p = partition(arr, l, h);
    quickSort(arr, l, p-1);
    quickSort(arr, p+1, h);
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    quickSort(array, 0, ARRAY_LENGTH - 1);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}