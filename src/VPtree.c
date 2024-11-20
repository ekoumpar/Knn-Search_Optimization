#include <../include/functions_c.h>
#include <stdio.h>
#include <math.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>


void buildVPTree(Matrix* matrix, VPNode** node, double* totalDistance, int* totalNodes, int** Kindex) {

    int dim = (int)matrix->cols;
    int N = (int)matrix->rows;

    if (N == 0) {
        (*node) = NULL;
        free(matrix->data);
        free(*Kindex);
        return;
    }

    //initialize first point as Vantage point
    *node = (VPNode*)malloc(sizeof(VPNode));
    if (*node == NULL) {
        printf("Problem with memory allocation in buildVPTree\n");
        return;
    }
    (*node)->point = (double*)malloc(dim * sizeof(double));
    if ((*node)->point == NULL) {
        printf("Problem with memory allocation in buildVPTree\n");
        free(*node);
        return;
    }
    for (int i = 0; i < dim; i++) {
        (*node)->point[i] = matrix->data[i];
        (*node)->index = (*Kindex)[0];
    }

    //case of a leaf
    if (N == 1) {
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->radius = 0;
        free(matrix->data);
        free(*Kindex);
        return;
    }

    //distanceBlas
    Matrix Vpoint;
    createMatrix(&Vpoint, 1, dim);
    for (int i = 0; i < dim; i++) {
        Vpoint.data[i] = (*node)->point[i];
    }

    Matrix distances;
    createMatrix(&distances, N - 1, 1);

    //ignore first point of array
    double* data_Matrix = matrix->data;
    if (matrix->data != NULL) {
        matrix->data += dim;
        matrix->rows = N - 1;
    }

    distanceBlas(matrix, &Vpoint, &distances);
    free(Vpoint.data);

    //calculate mean distance between corpus points
    double localTotalDistance = 0.0;
    int localTotalNodes = 0;

    cilk_for(int i = 0; i < (int)distances.rows; i++) {
        localTotalDistance += distances.data[i];
        localTotalNodes++;
    }
    cilk_sync;  
   
    // update 
    *totalDistance += localTotalDistance;
    *totalNodes += localTotalNodes;
    
    //calculate median of distances
    double median = quickMedian(distances.data, 0, N - 2);

    Matrix Left, Right;
    createMatrix(&Left, N - 1, dim);
    createMatrix(&Right, N - 1, dim);
    int* L_kindex = (int*)malloc(sizeof(int) * (N - 1));
    int* R_kindex = (int*)malloc(sizeof(int) * (N - 1));

    int left_count = 0, right_count = 0;
    for (int i = 0; i < (N - 1); i++) {
        if (distances.data[i] <= median) {
            for (int j = 0; j < dim; j++) {
                Left.data[left_count * dim + j] = matrix->data[i * dim + j];
            }
            L_kindex[left_count] = (*Kindex)[i + 1];
            left_count++; 
        }
        else {
            for (int j = 0; j < dim; j++) {
                Right.data[right_count * dim + j] = matrix->data[i * dim + j];
            }
            R_kindex[right_count] = (*Kindex)[i + 1];
            right_count++;  
        }
    }

    Left.rows = left_count;
    Right.rows = right_count;
    
    free(distances.data);
    free(data_Matrix);
    free(*Kindex);

    //node initialize
    (*node)->radius = median;
    (*node)->left = NULL;
    (*node)->right = NULL;

    //copy of data pointers for deallocation
    cilk_spawn buildVPTree(&Left, &(*node)->left, totalDistance, totalNodes, &L_kindex);
    cilk_spawn buildVPTree(&Right, &(*node)->right, totalDistance, totalNodes, &R_kindex);

    cilk_sync;
    return;
}

void freeVPTree(VPNode** node) {
    if (*node == NULL) return;

    freeVPTree(&(*node)->left);
    freeVPTree(&(*node)->right);

    free((*node)->point);
    free(*node);
    *node = NULL;
}

void printVPTree(VPNode* node, int level, int dim) {
    if (node == NULL) return;

    // levels of tree as tabs
    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    // print index,point and radius of node 
    printf("index %d ", node->index);
    printf("Point: (");
    for (int i = 0; i < dim; i++) {
        printf("%f", node->point[i]);
        if (i < dim - 1) printf(", ");
    }
    printf("), Radius: %f\n", node->radius);

    printVPTree(node->left, level + 1, dim);
    printVPTree(node->right, level + 1, dim);
}

