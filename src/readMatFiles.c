#include <../include/knn.h>

#include <stdio.h>
#include "matio.h"

void OpenFile(mat_t **file, const char* filename){

    *file=Mat_Open(filename, MAT_ACC_RDONLY);
    if (*file == NULL) {
        printf("Opening file failed\n");
        return;
       
    }
    

    printf("File opened succesfully\n");

}

void readMatrix(Matrix *matrix,const char* matrixName, mat_t *file) {

    matvar_t* arrayPr = Mat_VarRead(file, matrixName);

    if (arrayPr == NULL && arrayPr->data == NULL) {
        printf("Array pointer or data pointer in array is NULL.\n");
        Mat_VarFree(arrayPr);
    }

    double* dataPr = (double*)arrayPr->data;

    matrix->rows = (size_t)arrayPr->dims[0];
    matrix->cols = (size_t)arrayPr->dims[1];

    matrix->data = (double*)malloc(matrix->rows * matrix->cols * sizeof(double));
    if (matrix->data == NULL) {
        printf("Memory allocation for array->data failed.\n");
        Mat_VarFree(arrayPr);
    }
    for (int i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->data[i] = dataPr[i];
    }
    Mat_VarFree(arrayPr);
}

void CloseFile(mat_t** file) {
    int check=Mat_Close(*file);
    if (check != 0) {
        printf("Closing file failed!");
    }
}