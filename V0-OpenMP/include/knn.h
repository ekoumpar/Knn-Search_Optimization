#ifndef KNN_H
#define KNN_H

#include <stddef.h>
#include "matio.h"

typedef struct {
    double* data;
    size_t rows;
    size_t cols;
}Matrix;

void createMatrix(Matrix* matrix, size_t points, size_t dimensions);
void initiallizeMatrix(Matrix* matrix, size_t points, int start);
void printMatrix(const Matrix* matrix);
void OpenFile(mat_t** file, const char* filename);
void CloseFile(mat_t** file);
void saveMatrix(mat_t** file, int rows, int cols, double* data, char* name);
void createFile(mat_t** file);
void readMatrix(Matrix* matrix, const char* matrixName, mat_t* file);
int load_hdf5(const char* filename, const char* matname, Matrix* matrix);

void quickSelect(double* arr, double* index, int low, int high, int k, double* kneighbour, double* kindex);
void swap(double* a, double* b);
int partition(double* arr, int low, int high, double* index);

void distanceBlas(const Matrix* C, const Matrix* Q, Matrix* D);

void knnSearch(const Matrix* C, const Matrix* Q, int k, Matrix* K, Matrix* Kindex);

#endif
