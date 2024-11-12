#include <../include/functions_c.h>


// Swap function for QuickSelect
void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for QuickSelect
int partition1(double* arr, int low, int high) {
    double pivot = arr[high];
    int i = low;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    
    return i;
}

//Partition function for QuickSort
int partition2(double* arr, int low, int high) {

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

// Finds first k elements and sorts them with use of QuickSort
// returns them in kneighbour array

void quickSelect(double* arr, int low, int high, int k, double* kneighbour) {
    if (low <= high) {
        int static place = 0;
        if (place == 0) {
            place = k;
        }
        int pivotIndex = partition1(arr, low, high);

        if (pivotIndex-low == k-1) {
            // When k-th element is found, sort the k-elements array with quickSort
            // place the sorted array in kneighbour
            quickSort(arr, 0, place - 1);
            for (int i = 0; i < place; i++) {
                kneighbour[i] = arr[i];
            }
            return;
        }

        else if (pivotIndex-low>k-1)
            quickSelect(arr, low, pivotIndex - 1, k, kneighbour);
        else
            quickSelect(arr, pivotIndex + 1, high, k - (pivotIndex - low + 1), kneighbour);
    }
    
}

void quickSort(double *arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition2(arr, low, high);
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
    if ((high + 1) % 2 == 0) {
       median = (copy[high / 2] + copy[high / 2 + 1]) / 2;
       free(copy);
    return median;
    }
    else
      median = copy[(high + 1) / 2];
    free(copy);
    return median;    

}    
