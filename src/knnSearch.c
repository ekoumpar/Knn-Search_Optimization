#include <../include/knn.h>

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void knnSearch(const Matrix* C, const Matrix* Q, int k, Matrix* K, Matrix* Kindex){

    int N = (int)C->rows;
    int M = (int)Q->rows;
    int dim = (int)C->cols;

    // Get the number of available CPU cores
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }
    
    int qThreads = (int)(M/2) > (int)num_cores ? (int)num_cores : (int)(M/2);
    int cThreads = (int)(N/k) > (int)num_cores ? (int)num_cores : (int)(N/k);
    int rows_per_thread_Q = M / qThreads;
    int rows_per_thread_C = N / cThreads;

    #pragma omp parallel for collapse(2) num_threads(qThreads*cThreads)
    for(int i=0; i < qThreads; i++){
        for(int j=0; j<cThreads; j++){

            Matrix C_thread, Q_thread, K_thread, Kindex_thread;

            //Initialize matrix c for each thread
            C_thread.data = C->data + j * rows_per_thread_C * dim;
            C_thread.rows = (j == cThreads - 1) ? rows_per_thread_C + (N%cThreads) : rows_per_thread_C;
            C_thread.cols = dim;

            //Initialize matrix q for each thread
            Q_thread.data = Q->data + i * rows_per_thread_Q * dim;
            Q_thread.rows = (i == qThreads - 1) ? rows_per_thread_Q + (M%qThreads) : rows_per_thread_Q;
            Q_thread.cols = dim;

            //Initialize matrix k for each thread
            createMatrix(&K_thread, Q_thread.rows, k);

            //Initialize matrix kindex for each thread
            createMatrix(&Kindex_thread, Q_thread.rows, k);

            Matrix D, Index;
            createMatrix(&D, Q_thread.rows, C_thread.rows);
            createMatrix(&Index, 1, C_thread.rows);

            //Calculate the distance between each point in Q and C
            distanceBlas(&C_thread, &Q_thread, &D);

            //Find the k nearest neighbors
            int drows = (int)D.rows;
            int dCols = (int)D.cols;
            int start = j * rows_per_thread_C;
            for (int i = 0; i < drows; i++) {
                initiallizeMatrix(&Index, C_thread.rows, start+1);
                quickSelect(D.data + i*dCols, Index.data, 0, dCols - 1, k, K_thread.data + i*k, Kindex_thread.data + i*k);
            }

            #pragma omp critical
            {
                for (int n = 0; n < Q_thread.rows; n++) {
                    for (int l = 0; l < k; l++) {
                        K->data[(i * rows_per_thread_Q + n) * k + l] = K_thread.data[n * k + l];
                        Kindex->data[(i * rows_per_thread_Q + n) * k + l] = Kindex_thread.data[n * k + l];
                    }
                }
            }

            free(D.data);
            free(Index.data);
            free(K_thread.data);
            free(Kindex_thread.data);
        }
    }
}