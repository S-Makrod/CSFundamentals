#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 5

void insertionSort(int arr[], int n) {
    int num;

    for(int i = 0; i < n; i++){
        num = arr[i];
        for(int j = i-1; j >= 0; j--){
            if(num < arr[j]){
                arr[j+1] = arr[j];
                arr[j] = num;
            } else break;
        }
    }
}

int main(){
    int array[] = {33, 12, 45, 9, 24};
    insertionSort(array, ARRAY_LENGTH);

    // Print to double check
    for (int i = 0; i < ARRAY_LENGTH; i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}