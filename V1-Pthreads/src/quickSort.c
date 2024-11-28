#include <../include/functions_c.h>

// Swap function for QuickSort
void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

//Partition function for QuickSort
int partition(double* arr, int low, int high) {

    double pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(double *arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

double quickMedian(double* arr, int low, int high) {
    
    double* copy = NULL;
    double median;
    copy = (double*)malloc(sizeof(double) * (high + 1));
    if (copy == NULL) {
        printf("Problem with memory allocation in quickMedian");
    }
    for (int i = 0; i < high + 1; i++) {
        copy[i] = arr[i];
    }
    quickSort(copy, low, high);
    if ((high + 1) % 2 == 0){
       median = (copy[high / 2] + copy[high / 2 + 1]) / 2;
       free(copy);
       return median;
    }
    else
      median = copy[(high + 1) / 2];

    free(copy);
    return median;    
}    
