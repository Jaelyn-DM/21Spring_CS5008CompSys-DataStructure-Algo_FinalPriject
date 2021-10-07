#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 100000

//This function helps us perform a quick swap between two elements.
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

//Helper function to print out arrays if needed, especially during troubleshoot
void printArray(int array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int MERGE_COMPARE = 0;

// merge sort
void merge (int arr[], int left, int mid, int right, int temp[]);
void mergeSort(int arr[], int left, int right, int temp[]) {
    if (left < right) // recursion stops when there's only one number left
    {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid ,temp); // left recursion to split the array
        mergeSort(arr, mid + 1, right, temp); // right recusrion to split the array
        merge(arr, left, mid, right, temp); // merge
    }  
}

void merge(int arr[], int left, int mid, int right, int temp[]) {
    int i = left; // index of sorted left half array
    int j = mid + 1; // index of sorted right half array
    int t = 0; // point at the current index of temp array
    // copy the sorted left and right half to temp array, until one half finished
    while (i <= mid && j <= right)
    {
        MERGE_COMPARE++;
        if (arr[i] <= arr[j]) // if element in left half is smaller than element in right half, copy it to temp array and move pointers
        {
            temp[t] = arr[i];
            t++;
            i++;
        } else { // else copy the right half element to temp array and move pointers
            temp[t] = arr[j];
            t++;
            j++;
        }
        
    }
    // copy any remain elements to temp at once
        while (i <= mid) // if there are elements left in left half
    {
        temp[t] = arr[i];
        t++;
        i++;
    }
    while (j <= right) // if there are elements left in right half
    {
        temp[t] = arr[j];
        t++;
        j++;
    }
    // copy elements in temp array to array
        t = 0;
    int tempLeft = left;
    while (tempLeft <= right)
    {
        arr[tempLeft] = temp[t];
        t++;
        tempLeft++;
    }   
}

int main() {
    for (int i = 0; i < 10; i++){
        printf("Round %d\n", i+1);

        clock_t start, end;
        double time_used;

        int merge_array[SIZE];
        int merge_temp[SIZE];

        int i;
        for (i = 0; i < SIZE; i ++){
            merge_array[i] = i;
            }
        for (i = SIZE - 1; i > 1; i--) {
            int j = rand() % i;
            int tmp = merge_array[j];
            merge_array[j] = merge_array[i];
            merge_array[i] = tmp;
        }

        int array_length = sizeof(merge_array) / sizeof(merge_array[0]);

        start = clock();
        mergeSort(merge_array, 0, array_length - 1, merge_temp);
        end = clock();
        time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("Merge Sort time usage: %f\n", time_used);
    }
}
