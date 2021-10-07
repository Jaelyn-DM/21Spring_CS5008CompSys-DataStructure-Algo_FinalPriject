#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

// insertion sort
void InsertionSort(int arr[], int length)
{
    int insertVal = 0; // store the number being inserted
    int insertIndex = 0; // store the index of the number being inserted
    for (int i = 1; i < length; i++)
    {
        insertVal = arr[i]; // define the number being inserted
        insertIndex = i - 1; // find the index of the number before the one being inserted
        // find the proper index for insertVal
        // insertIndex >= 0 prevents index out of boundary
        // insertVal < arr[insertIndex] looking for the index to insert insertVal
        while (insertIndex >= 0 && insertVal < arr[insertIndex]) // if insertVal < arr[insertIndex](the previous number)
        {
            arr[insertIndex + 1] = arr[insertIndex]; // move the previous number one index back
            insertIndex--; // continue looking for the one before that
        }
        // when quit the while loop, the index has been found
        arr[insertIndex + 1] = insertVal;
    }
}

int main() {
    for (int i = 0; i < 10; i++){
        printf("Round %d\n", i+1);

        clock_t start, end;
        double time_used;

        int array[SIZE];

        int i;
        for (i = 0; i < SIZE; i ++){
            array[i] = i;
            }
        for (i = SIZE - 1; i > 1; i--) {
            int j = rand() % i;
            int tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
        }

        int length = sizeof(array) / sizeof(array[0]);

        printf("Insertion Sort Result: \n");
        start = clock();
        InsertionSort(array, length);
        end = clock();
        time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Time used: %f\n", time_used);
    }
}