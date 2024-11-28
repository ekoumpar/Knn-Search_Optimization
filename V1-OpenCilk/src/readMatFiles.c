#include <../include/functions_c.h>

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
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            matrix->data[i * matrix->cols + j] = dataPr[j * matrix->rows + i];
        }
    }
    Mat_VarFree(arrayPr);
}

void CloseFile(mat_t** file) {
    int check=Mat_Close(*file);
    if (check != 0) {
        printf("Closing file failed!");
    }
}

void CreateFile(mat_t** file) {

    *file = Mat_CreateVer("output.mat", NULL, MAT_FT_MAT5);
    if (*file == NULL) {
        fprintf(stderr, "Σφάλμα: Αδυναμία δημιουργίας MAT αρχείου.\n");
    }
}


void saveMatrix(mat_t** file, int rows, int cols, double* data, const char* name) {

    size_t dims[2] = { (size_t)rows,(size_t)cols };
    double* dataPr = (double*)malloc(rows * cols * sizeof(double));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dataPr[j * rows + i] = data[i * cols + j];
        }
    }
    matvar_t* matvar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, dataPr, 0);
    if (matvar == NULL) {
        fprintf(stderr, "Σφάλμα: Αδυναμία δημιουργίας MAT μεταβλητής.\n");
        Mat_Close(*file);
        return;
    }

    if (Mat_VarWrite(*file, matvar, MAT_COMPRESSION_NONE) != 0) {
        fprintf(stderr, "Σφάλμα: Αδυναμία εγγραφής της μεταβλητής στο MAT αρχείο.\n");
        return;
    }
    free(dataPr);
    Mat_VarFree(matvar);

}
