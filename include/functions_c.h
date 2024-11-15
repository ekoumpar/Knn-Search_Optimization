#ifndef FUNCTIONS_C_H
#define FUNCTIONS_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "matio.h"
#include "vptree.h"

typedef struct {
    double* data;
    size_t cols;
    size_t rows;
}Matrix;

void createMatrix(Matrix* matrix, size_t points, size_t dimensions);
void randomMatrix(Matrix* matrix, size_t points, size_t dimensions);
void printMatrix(const Matrix* matrix);
void OpenFile(mat_t** file, const char* filename);
void readMatrix(Matrix* matrix, const char* matrixName, mat_t* file);
void CloseFile(mat_t** file);
int load_hdf5(const char* filename, const char* matname, Matrix* matrix);

void buildVPTree(Matrix* matrix, VPNode** node, double* totalDistance, int* totalNodes);
void freeVPTree(VPNode** node);
void printVPTree(struct VPNode* node, int level, int dim);

void quickSelect(double* arr, int low, int high, int k, double* kneighbour);
void swap(double* a, double* b);
void quickSort(double* arr, int low, int high);
int partition2(double* arr, int low, int high);
int partition(double* arr, int low, int high);
double quickMedian(double* arr, int low, int high);

void distanceBlas(const Matrix* C, const Matrix* Q, Matrix* D);

#ifdef __cplusplus
}
#endif

#endif 