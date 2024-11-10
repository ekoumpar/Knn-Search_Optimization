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
   
    //get Corpus
    readMatrix(&C, "CORPUS", file);

    //for debugging
    printf("---------------Corpus-Set----------------");
    printMatrix(&C);
    
   //after bulidVPTree C is deleted!
    VPNode* Corpus = NULL;
    buildVPTree(&C, &Corpus);

    //for debugging
    printf("\n\n");
    printVPTree(Corpus, 0, C.cols);
    printf("\n\n");


    
    readMatrix(&Q, "QUERY", file);
    printf("Enter the number of nearest neighbours: ");
     if (scanf("%zu", &k) != 1) {
          printf("Invalid input for 'k'.\n");
          return -1;
        }
    printf("---------------Query-Set-----------------");
    printMatrix(&Q);

    /*
    size_t corpus = C.rows;
    size_t query = Q.rows;
    createArray(&D, query, corpus);
    createArray(&K, query, k);
   
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
    */
    //free(K.data);
    //free(D.data);
    

    free(Q.data);
    freeVPTree(&Corpus);
    CloseFile(&file);
    return 0;
}
