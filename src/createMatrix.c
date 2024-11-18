#include <../include/functions_c.h>

#include <stdio.h>
#include <stdlib.h>

void createMatrix(Matrix* matrix, size_t points, size_t dimensions){
    matrix->rows = points;
    matrix->cols = dimensions;
    matrix->data = (double*)malloc(points*dimensions*sizeof(double));
    if(matrix->data == NULL){
        printf("Problem with memory allocation in createMatrix");
    }
}

void printMatrix(const Matrix* matrix) {
    double* A = matrix->data;
    size_t rows = matrix->rows;
    size_t cols = matrix->cols;

    printf("\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%10.5lf ", A[i * cols + j]);
        }
        printf("\n");
    }
}

