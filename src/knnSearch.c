#include <../include/knn.h>

#include <cilk/cilk.h>
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

    cilk_for (int i = 0; i < qThreads; i++) {

        Matrix Q_thread;

        //Initialize matrix q for each thread
        Q_thread.data = Q->data + i * rows_per_thread_Q * dim;
        Q_thread.rows = (i == qThreads - 1) ? rows_per_thread_Q + (M % qThreads) : rows_per_thread_Q;
        Q_thread.cols = dim;

        Matrix allK, allKindex;
        createMatrix(&allK, Q_thread.rows, k * cThreads);
        createMatrix(&allKindex, Q_thread.rows, k * cThreads);
        int index = i * k * rows_per_thread_Q;

        cilk_for (int j = 0; j < cThreads; j++) {

            Matrix C_thread;

            //Initialize matrix c for each thread
            C_thread.data = C->data + j * rows_per_thread_C * dim;
            C_thread.rows = (j == cThreads - 1) ? rows_per_thread_C + (N % cThreads) : rows_per_thread_C;
            C_thread.cols = dim;

            Matrix D, Index, K_thread, Kindex_thread;
            createMatrix(&D, Q_thread.rows, C_thread.rows);
            createMatrix(&Index, 1, C_thread.rows);
            createMatrix(&K_thread, Q_thread.rows, k);
            createMatrix(&Kindex_thread, Q_thread.rows, k);

            //Calculate the distance between each point in Q and C
            distanceBlas(&C_thread, &Q_thread, &D);

            //Find the k nearest neighbors
            int drows = (int)D.rows;
            int dCols = (int)D.cols;
            int start = j * rows_per_thread_C;
            for (int m = 0; m < drows; m++) {
                initiallizeMatrix(&Index, C_thread.rows, start + 1);
                quickSelect(D.data + m * dCols, Index.data, 0, dCols - 1, k, K_thread.data + m * k, Kindex_thread.data + m * k);
                for (int n = 0; n < k; n++) {
                    allK.data[m * k * cThreads + j * k + n] = K_thread.data[m * k + n];
                    allKindex.data[m * k * cThreads + j * k + n] = Kindex_thread.data[m * k + n];
                }
            }

            free(D.data);
            free(Index.data);
            free(K_thread.data);
            free(Kindex_thread.data);
        }

        for (int m = 0; m < Q_thread.rows; m++) {
            quickSelect(allK.data + m * k * cThreads, allKindex.data + m * k * cThreads, 0, k * cThreads - 1, k, K->data + m * k + index, Kindex->data + m * k + index);
        }

        free(allK.data);
        free(allKindex.data);
    }
}