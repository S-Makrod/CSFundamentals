#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 5

void bubbleSort(int arr[], int n) {
    int num;

    for(int i = 0; i < n; i++){
        for(int j = 1; j  < n-i; j++){
            if(arr[j-1] > arr[j]){
                num = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = num;
            }
        }
    }
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    bubbleSort(array, ARRAY_LENGTH);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}