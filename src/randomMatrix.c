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

void randomMatrix(Matrix* matrix, size_t points, size_t dimensions){
    matrix->rows = points;
    matrix->cols = dimensions;
    matrix->data = (double*)malloc(points*dimensions*sizeof(double));
    if(matrix->data == NULL){
        printf("Problem with memory allocation in randomMatrix");
    }

    for(size_t i=0; i<points*dimensions; i++){
        matrix->data[i] = ((double)rand()/RAND_MAX)*100;
    }
}
