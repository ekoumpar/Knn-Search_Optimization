//#include <mat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void quickSelect(double* arr, int low, int high, int k, double* kneighbour);
void swap(double* a, double* b);
void quickSort(double* arr, int low, int high);
int partition2(double* arr, int low, int high);
int partition(double* arr, int low, int high);

int main() {
    
    //knnsearch("my3Dmatrices.mat", "my3Dmatrices.mat", 5, 2);


    int M=5;//number of queries
    int d = 6;//number of dimensions

    //example matrix of distances 
    double distances[M][d];
    srand(time(0));
    for (int j = 0; j < M; j++) {
        printf("\n%d. row is: ", j + 1);
        for (int i = 0; i < d; i++) {
             distances[j][i] = rand()/1000.0;
              printf("%f ", distances[j][i]);

    }
    }
   

    double** kneighbour=NULL;// pointer to k neighbours matrix
    int k = 3; //kth element to search
    
    //memory allocation for k elements
    kneighbour = (double**)malloc(sizeof(double)*M);
    for (int i = 0; i < M; i++) {
        kneighbour[i] = (double*)malloc(sizeof(double) * k);
    }
    if (kneighbour == NULL) {
        printf("Error occured by malloc");
        return 0;
    }

    //Sorting of kth elements 
    for (int i = 0; i < M; i++) {
        quickSelect(distances[i], 0, d- 1, k, kneighbour[i]);
    }
    
    //print sorted k-first elements
    if (kneighbour != NULL) {
        for (int j = 0; j < M; j++) {
            printf("\n%d. row is: [ ", j + 1);
            for (int i = 0; i < k; i++) {
                printf(" %f",kneighbour[j][i]);
            }
            printf(" ]");
        }
    }
    else 
        printf("Error in finding kneighbours.\n");
    
    //deallocation of memory
    for (int i = 0; i < M; i++) {
        free(kneighbour[i]);
    }
    free(kneighbour);

    return 0;


}
/*
void knnsearch(char** queryfile, char** corpusfile, int k, int dimensions) {


    //get Q
    while ((Q = readQ(queryfile)) == NULL) {
        printf("Error reading query file!");
        exit(1);
    }
    //get C
    while ((C = readQ(corpusfile)) == NULL) {
        printf("Error reading corpus file!");
        exit(1);
    }

}*/
    /*
    void quickselect(double** distances, int n, int m, double** kneighbour, int k) {
        // n m dimensions of partioned threaded queries( N/CTHREAD & M/QTHREAD)

        for (int i = 0; i < n; i++) {
            kthSmallest(distances[i], 0, m - 1, kneighbour[i]);
        }


    }
    */



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


/*

//read matrix Q from mat file
mxArray* readQ(char ** filename) {

    MATFile* file;
    mxArray* Q;
    

    //open mat file
    file = matOpen(filename[], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // get query
    Q = matGetVariable(file, "query");
    if (Q == NULL) {
        printf("Query not found\n");
        return NULL;
    }
    return Q;    
}

//read matrix C from mat file
mxArray* readC(char ** filename) {

    MATFile* file;
    mxArray* C;

    //open mat file
    file = matOpen(filename[], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // get corpus
    C = matGetVariable(file, "corpus");
    if (C == NULL) {
        printf("Corpus not found\n");
        return NULL;
    }
    return C;

}


*/



