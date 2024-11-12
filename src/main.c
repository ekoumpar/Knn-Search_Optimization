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
    const char* filename="small.mat";
    mat_t *file = NULL;

    OpenFile(&file, filename);

    readMatrix(&C, "CORPUS", file);
    readMatrix(&Q, "QUERY", file);

    
    printf("Enter the number of nearest neighbours: ");
    if (scanf("%zu", &k) != 1) {
        printf("Invalid input for 'k'.\n");
        return -1;
    }
   

    printf("---------------Corpus-Set----------------");
    printMatrix(&C);
    
    
    printf("---------------Query-Set-----------------");
    printMatrix(&Q);
    
    //Calculate k nearest neighbours
    createMatrix(&K, Q.rows, k);
    knnSearch(&C, &Q, k, &K);

    printf("---------------K neighbours----------------");
    printMatrix(&K);
  
    free(K.data);
    free(C.data);
    free(Q.data);
    
    CloseFile(&file);
    return 0;
}
