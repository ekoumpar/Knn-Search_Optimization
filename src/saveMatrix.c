#include <../include/knn.h>

#include <stdio.h>
#include "matio.h"

void saveMatrix(mat_t **file, int rows, int cols, double* data, char * name) {
      
    size_t dims[2] = { (size_t)rows,(size_t)cols };
    double* dataPr= (double*)malloc(rows * cols * sizeof(double));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dataPr[j * rows + i] = data[i * cols + j];
        }
    }
    matvar_t* matvar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, dataPr, 0);
    if (matvar == NULL) {
        fprintf(stderr, "Error: Problem with creating mat variable.\n");
        Mat_Close(*file);
        return ;
    }

    if (Mat_VarWrite(*file, matvar, MAT_COMPRESSION_NONE) != 0) {
        fprintf(stderr, "Error: Problem with writing mat variable.\n");
        return;
    }
    free(dataPr);
    Mat_VarFree(matvar);
 

}


