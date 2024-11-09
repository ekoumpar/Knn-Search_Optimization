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
void randomMatrix(Matrix* matrix, size_t points, size_t dimensions);
void printMatrix(const Matrix* matrix);
void OpenFile(mat_t** file, const char* filename);
void readMatrix(Matrix* matrix, const char* matrixName, mat_t* file);
void CloseFile(mat_t** file);

void quickSelect(double* arr, int low, int high, int k, double* kneighbour);
void swap(double* a, double* b);
void quickSort(double* arr, int low, int high);
int partition2(double* arr, int low, int high);
int partition(double* arr, int low, int high);

void distanceBlas(const Matrix* C, const Matrix* Q, Matrix* D);

#endif
