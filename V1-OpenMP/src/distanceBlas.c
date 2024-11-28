#include <../include/functions_c.h>

#include <stdlib.h>
#include <math.h>
#include <cblas.h>

void distanceBlas(const Matrix* C, const Matrix* Q, Matrix* D){
    int N = (int)C->rows;
    int M = (int)Q->rows;
    int dim = (int)C->cols;

    double *C2 = (double*)malloc(N*sizeof(double));
    double *Q2 = (double*)malloc(M*sizeof(double));

    //Element-wise square of C and Q
    for(int i=0; i < N; i++){
        C2[i] = 0;
        for(int j=0; j<dim; j++){
            C2[i] += C->data[i*dim +j] * C->data[i*dim +j];
        }
    }
    for(int i=0; i < M; i++){
        Q2[i] = 0;
        for(int j=0; j<dim; j++){
            Q2[i] += Q->data[i*dim +j] * Q->data[i*dim +j];
        }
    }

    double *CQ = (double*)malloc(N*M*sizeof(double));

    //Matrix multiplication C*Q^T using cblas
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                N, M, dim, -2.0, C->data, dim,
                Q->data, dim, 0.0, CQ, M);

    //Add C2 and Q2 to CQ and calculate the square root of all
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            CQ[i*M + j] += C2[i] + Q2[j];

            //Ensure no negative distances
            if(CQ[i*M + j] < 0)
                CQ[i*M + j] = 0.0;

            D->data[j*N + i] = sqrt(CQ[i*M + j]);
        }
    }

    free(C2);
    free(Q2);
    free(CQ);
    
}
