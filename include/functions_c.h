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
void printMatrix(const Matrix* matrix);

void OpenFile(mat_t** file, const char* filename);
void CloseFile(mat_t** file);
void CreateFile(mat_t** file);
void readMatrix(Matrix* matrix, const char* matrixName, mat_t* file);
void saveMatrix(mat_t** file, int rows, int cols, double* data,const char* name);

int load_hdf5(const char* filename, const char* matname, Matrix* matrix);

void buildVPTree(Matrix* matrix, VPNode** node, double* totalDistance, int* totalNodes, int** Kindex);
void freeVPTree(VPNode** node);
void printVPTree(struct VPNode* node, int level, int dim);

void quickSort(double* arr, int low, int high);
double quickMedian(double* arr, int low, int high);
int partition(double* arr, int low, int high);
void swap(double* a, double* b);

void distanceBlas(const Matrix* C, const Matrix* Q, Matrix* D);

#ifdef __cplusplus
}
#endif

#endif 
