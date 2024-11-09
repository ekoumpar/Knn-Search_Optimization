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
    const char* filename = "data.mat";
    mat_t *file = NULL;

    
    //open file 
    file=Mat_Open(filename, MAT_ACC_RDONLY);
    if (file == NULL) {
        printf("Error in opening given Matfile");
    }

    //get Corpus
    matvar_t* arrayPr = Mat_VarRead(file, "CORPUS");
    if (arrayPr == NULL && arrayPr->data == NULL) {
        printf("Array pointer or data pointer in array is NULL.\n");
        Mat_VarFree(arrayPr);
    }

    double* dataPr = (double*)arrayPr->data;
    
    C.rows = (size_t)arrayPr->dims[0];
    C.cols = (size_t)arrayPr->dims[1];
    
    C.data = (double*)malloc(C.rows*C.cols * sizeof(double));
    if (C.data == NULL) {
        printf("Memory allocation for array->data failed.\n");
        Mat_VarFree(arrayPr);
    }
    for (int i = 0; i < C.rows * C.cols; i++) {
        C.data[i] = dataPr[i];
    }
    Mat_VarFree(arrayPr);

    //get Query
    arrayPr = Mat_VarRead(file, "QUERY");
    if (arrayPr == NULL && arrayPr->data == NULL) {
        printf("Array pointer or data pointer in array is NULL.\n");
        Mat_VarFree(arrayPr);
    }

    dataPr = (double*)arrayPr->data;

    Q.rows = (size_t)arrayPr->dims[0];
    Q.cols = (size_t)arrayPr->dims[1];

    Q.data = (double*)malloc(Q.rows * Q.cols * sizeof(double));
    if (Q.data == NULL) {
        printf("Memory allocation for array->data failed.\n");
        Mat_VarFree(arrayPr);
    }
    for (int i = 0; i < Q.rows *Q.cols; i++) {
        Q.data[i] = dataPr[i];
    }
    Mat_VarFree(arrayPr);

    
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
    //printMatrix(&C);
    
    
    printf("---------------Query-Set-----------------");
    //printMatrix(&Q);
    
    //Calculate Distances
    distanceBlas(&C, &Q, &D);
    printf("---------------Distances-----------------");
    //printMatrix(&D);
        
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
    
    Mat_Close(file);
    return 0;
}

