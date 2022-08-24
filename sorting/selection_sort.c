#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 5

void selectionSort(int arr[], int n) {
    int min, index, temp;

    for(int i = 0; i < n; i++){
        min = arr[i];
        index = i;
        for(int j = i; j < n; j++){
            if(arr[j] < min){
                index = j;
                min = arr[j];
            }
        }
        temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    selectionSort(array, ARRAY_LENGTH);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}