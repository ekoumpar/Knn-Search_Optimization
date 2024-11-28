#include <../include/knn.h>


// Swap function for QuickSelect
void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for QuickSelect
int partition(double* arr, int low, int high,double *index){

    double pivot = arr[high];
    int i = low;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            swap(&index[i], &index[j]);
            
            i++;
  
        }
    }

    swap(&arr[i], &arr[high]);
    swap(&index[i], &index[high]);
    
    return i;
}


// Finds first k elements and sorts them with use of QuickSort
// returns them in kneighbour array
void quickSelect(double* arr, double* index, int low, int high, int k, double* kneighbour, double* kindex){
    if (low <= high) {
        int static place = 0;
        //initialization
        if (place == 0) {
            place = k;
        }

        int pivotIndex = partition(arr, low, high, index);

        //k elements found
        if (pivotIndex - low == k - 1) {
     
            for (int i = 0; i < place; i++) {
                kneighbour[i] = arr[i];
                kindex[i] = index[i];
            }
            return;
        }

        else if (pivotIndex - low > k - 1)
            quickSelect(arr,index, low, pivotIndex - 1, k, kneighbour,kindex);
        else
            quickSelect(arr,index, pivotIndex + 1, high, k - (pivotIndex - low + 1), kneighbour, kindex);
    }
    
}

