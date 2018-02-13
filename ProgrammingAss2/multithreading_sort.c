/*
 * Project 2: Shell Interface and Multithreading
 * Felix Adrian Lucaciu 27397941
 * Michael Magnabosco   27189737
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>


//Input Array needed to be sorted
int InputArraySize = 40;
int InputArray [] = {75, 25, 100, 24, 19, 67, 45, 88, 70, 96,
    41, 36, 72, 150, 125, 25, 77, 200, 90, 166,
    139, 55, 31, 8, 29, 119, 126, 137, 34, 62,
    135, 121, 108, 197, 45, 35, 24, 1, 16, 43};

int sortedArray[40];

typedef struct{
    int from_index;
    int to_index;
}parameters;

//Swaps the 2 elements
void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void display(int arr[], int arraySize) {
    int i;
    for (i = 0; i < arraySize; i++) {
        //printf("size of the arr is: %d \n", sizeof(arr)/4);
        printf("%d ", arr[i]);
    }
    
    printf("\nExiting Display function \n");
    
}

// Sorting Function: using Bubblesort
void * Sorter(parameters * Data){
    int StartPoint = Data->from_index;
    int StoppingPoint = Data->to_index;
    
    for (int i = 0; i < (StoppingPoint-StartPoint)-1; i++) {
        for (int j = StartPoint; j < StoppingPoint-i-1; j++) {
            if (InputArray[j] > InputArray[j+1]) {
                swap(&InputArray[j], &InputArray[j+1]);
            }
        }
    }
    
    printf("Displaying Input array after it's been sorted\n");
    display(InputArray, InputArraySize);
    pthread_exit(NULL);
    //Need to add exit here
}

void *Merger(parameters * Data) {
    int i, j, k;
    int size1 =Data->from_index;
    int size2 = Data->to_index;
    
    i = 0;
    j = size1;
    k = 0;
    
    while (i <size1 && j <size2) {
        if (InputArray[i] <= InputArray[j]) {
            sortedArray[k] = InputArray[i];
            i++;
        }
        else
        {
            sortedArray[k] = InputArray[j];
            j++;
        }
        k++;
    }
    // Store remaining elements of first array
    while (i < size1)
        sortedArray[k++] = InputArray[i++];
    
    // Store remaining elements of second array
    while (j < size2)
        sortedArray[k++] = InputArray[j++];
    
    display(sortedArray, InputArraySize);
    pthread_exit(NULL);   //exit here.
}

int main(void) {
    
    pthread_t t1, t2, t3;
    parameters * Data;
    printf("size of the input array is %d \n", sizeof(InputArray)/4);
    display(InputArray, InputArraySize);
    int size1 = InputArraySize/2;
    int size2 = InputArraySize/2;
    int size3 = InputArraySize;
    
    /* Creating Thread 1, first child thread */
    Data = malloc(sizeof(*Data));
    Data->from_index = 0;
    Data->to_index = size1;
    pthread_create(&t1, 0, Sorter, Data);
    sleep(1);
    free(Data);
    
    /* Creating Thread 2, second child thread */
    Data = malloc(sizeof(*Data));
    Data->from_index = size2;
    Data->to_index = size3;
    pthread_create(&t2, 0, Sorter, Data);
    
    //free(Data);
    
    /* Joining two children thread to main parent thread */
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);// exit doesn't go here.
    
    /* Creating Thread 3, parent thread */
    //Data = malloc(sizeof(*Data));
    Data->from_index = size1;
    Data->to_index = size3;
    pthread_create(&t3, 0, Merger, Data);
    sleep(1);
    free(Data);
    
    return 0;
}

