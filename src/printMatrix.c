#include <../include/functions_c.h>

#include <stdio.h>

void printMatrix(const Matrix* matrix){
    double *A = matrix->data;
    size_t rows = matrix->rows;
    size_t cols = matrix->cols;

    printf("\n");
    for(size_t i=0; i<rows; i++){
        for(size_t j=0; j<cols; j++){
            printf("%10.2lf ", A[i*cols + j]);
        }
        printf("\n");
    }
}
