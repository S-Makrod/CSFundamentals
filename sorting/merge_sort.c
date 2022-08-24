#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 5

void merge(int arr[], int l, int m, int r) {
    int s1 = m-l+1, s2 = r-m;
    int arr1[s1], arr2[s2];

    for(int i = 0; i < s1; i++) arr1[i] = arr[l+i];
    for(int i = 0; i < s2; i++) arr2[i] = arr[m+1+i];

    int j = 0, i = 0, k = l;

    while(i < s1 && j < s2) {
        if(arr1[i] > arr2[j]) {
            arr[k] = arr2[j];
            j++;
        } else {
            arr[k] = arr1[i];
            i++;
        }
        k++;
    }

    while(i < s1) {
        arr[k] = arr1[i];
        i++;
        k++;
    }

    while (j < s2) {
        arr[k] = arr2[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if(l >= r) return;
    int m=(l+r)/2;

    mergeSort(arr, l, m);
    mergeSort(arr, m+1, r);

    merge(arr, l, m, r);
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    mergeSort(array, 0, ARRAY_LENGTH - 1);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}