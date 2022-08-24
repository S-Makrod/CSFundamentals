#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_LENGTH 100

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

int jumpSearch(int arr[], int n, int val) {
    int jump = 5, i;
    for(i = 0; i < n; i += jump) if(arr[i] > val) break;

    if(i == 0) return arr[i] == val;
    if(i >= n) return 0;
    for(int j = i-jump; j < i; j++) if(arr[j] == val) return 1;
    return 0;
}

int main(){
    int array[] = {5484, -2372, -480, -779, -7321, -555, -6175, 9788, -4434, -7484, 1250, -6516, 3799, -8796, -3229, 9431,
        1825, -9011, -4191, 6460, -191, 3627, 1610, 8283, -9027, -2350, 9863, -4495, -4163, 1265, -1270, 9921, 3682, -1158,
        -5728, -3030, 9068, 9961, 2617, 9227, -4148, -4870, 7377, 1615, -2991, 3776, 7343, 9247, -7245, -5392, 5305, -6180,
        8346, 8444, 9237, -700, -4809, 3081, 2778, 3232, -6864, -6122, -8944, 8383, -6900, -7736, -7242, -5852, 2044, -8062,
        -7668, -8485, -3369, -1587, -2229, -2559, 8010, 6921, 4366, -1875, 8202, -1102, -9520, 1954, -3616, 8713, -5470, 7593,
        8550, 7750, 9432, 4000, -878, 8508, -6155, -3594, 2213, 8682, -6210, -9843};

    // I wanted to do a large array but I don't want to sort all those numbers so I copied over my quicksort to do that
    quickSort(array, 0, ARRAY_LENGTH - 1);

    if(jumpSearch(array, ARRAY_LENGTH, 8508)) {
        printf("8508 is in the array\n");
    } else {
        printf("8508 is not in the array\n");
    }

    if(jumpSearch(array, ARRAY_LENGTH, -8508)) {
        printf("-8508 is in the array\n");
    } else {
        printf("-8508 is not in the array\n");
    }

    return 0;
}