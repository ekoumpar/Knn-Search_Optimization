#include <../include/knn.h>

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

void initiallizeMatrix(Matrix* matrix, size_t points, int start){
    for (size_t j = 0; j < points; j++) {
        matrix->data[j] =start;
        start++;
    }  
}
