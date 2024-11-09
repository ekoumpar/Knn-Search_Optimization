#include <../include/knn.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matio.h"

int main(){
    srand(time(NULL));
    
    Matrix C, Q, D, K;
    size_t k;
    const char* filename="data.mat";
    mat_t *file = NULL;

    OpenFile(&file, filename);

    readMatrix(&C, "CORPUS", file);
    readMatrix(&Q, "QUERY", file);

    
    printf("Enter the number of nearest neighbours: ");
     if (scanf("%zu", &k) != 1) {
          printf("Invalid input for 'k'.\n");
          return -1;
        }
    
   
    size_t corpus = C.rows;
    size_t query = Q.rows;
    createArray(&D, query, corpus);
    createArray(&K, query, k);
   

    printf("---------------Corpus-Set----------------");
    printMatrix(&C);
    
    
    printf("---------------Query-Set-----------------");
    printMatrix(&Q);
    
    //Calculate Distances
    distanceBlas(&C, &Q, &D);
    printf("---------------Distances-----------------");
    printMatrix(&D);
        
    //Sorting of kth elements
    int rows = (int)D.rows;
    int dCols = (int)D.cols;
    int kCols = (int)K.cols; 
    for (int i = 0; i < rows; i++) {
        quickSelect(D.data + i*dCols, 0, dCols - 1, kCols, K.data + i*kCols);
    }

    printf("---------------K neighbours----------------");
    printMatrix(&K);
    
 

  
    free(K.data);
    free(C.data);
    free(Q.data);
    free(D.data);
    
    CloseFile(&file);
    return 0;
}
