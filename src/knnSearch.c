#include <../include/knn.h>

#include <pthread.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int index;
    Matrix* C;
    Matrix* Q;
    Matrix* K;
    Matrix* Kindex;
    int k;
    int cId;
} ThreadData;

void* knnSearchThread(void* arg){
    ThreadData* thread = (ThreadData*)arg;

    Matrix D, Index;
    createMatrix(&D, thread->Q->rows, thread->C->rows);
    createMatrix(&Index, 1, thread->C->rows);

    //Calculate the distance between each point in Q and C
    distanceBlas(thread->C, thread->Q, &D);

    //Find the k nearest neighbors
    int drows = (int)D.rows;
    int dCols = (int)D.cols;
    int start = thread->cId;
    for (int i = 0; i < drows; i++) {
        initiallizeMatrix(&Index, thread->C->rows, start+1);
        quickSelect(D.data + i*dCols,Index.data, 0, dCols - 1, thread->k, thread->K->data + i*thread->k, thread->Kindex->data + i * thread->k);
    }

    free(D.data);
    free(Index.data);

    pthread_exit(NULL);
}

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

    pthread_t threads[qThreads*cThreads];
    ThreadData thread_data[qThreads*cThreads];

    for(int i=0; i < qThreads; i++){
        for(int j=0; j<cThreads; j++){

            thread_data[i*cThreads+j].index = i*cThreads+j;

            //Initialize matrix c for each thread
            thread_data[i*cThreads+j].C = (Matrix*)malloc(sizeof(Matrix));
            thread_data[i*cThreads+j].C->data = C->data + j * rows_per_thread_C * dim;
            thread_data[i*cThreads+j].C->rows = (j == cThreads - 1) ? rows_per_thread_C + (N%cThreads) : rows_per_thread_C;
            thread_data[i*cThreads+j].C->cols = dim;

            //Initialize matrix q for each thread
            thread_data[i*cThreads+j].Q = (Matrix*)malloc(sizeof(Matrix));
            thread_data[i*cThreads+j].Q->data = Q->data + i * rows_per_thread_Q * dim;
            thread_data[i*cThreads+j].Q->rows = (i == qThreads - 1) ? rows_per_thread_Q + (M%qThreads) : rows_per_thread_Q;
            thread_data[i*cThreads+j].Q->cols = dim;

            //Initialize matrix k for each thread
            thread_data[i*cThreads+j].K = (Matrix*)malloc(sizeof(Matrix));
            createMatrix(thread_data[i*cThreads+j].K, thread_data[i*cThreads+j].Q->rows, k);

            //Initialize matrix kindex for each thread
            thread_data[i * cThreads + j].Kindex = (Matrix*)malloc(sizeof(Matrix));
            createMatrix(thread_data[i * cThreads + j].Kindex, thread_data[i * cThreads + j].Q->rows, k);

            thread_data[i*cThreads+j].k = k;
            thread_data[i * cThreads + j].cId = j* rows_per_thread_C;

            pthread_create(&threads[i*cThreads+j], NULL, knnSearchThread, (void*)&thread_data[i*cThreads+j]);
        }
    }

    int rows =0;
    for (int i = 0; i < qThreads; i++) {
    
        int addRows = (int)thread_data[i*cThreads].K->rows;
        Matrix allK, allKindex;
        createMatrix(&allK, addRows, k*cThreads);
        createMatrix(&allKindex, addRows, k * cThreads);

        for (int j = 0; j < cThreads; j++) {
            pthread_join(threads[i*cThreads+j], NULL);
            for(int n=0; n<addRows; n++){
                for(int l=0; l<k; l++){
                    allK.data[n*k*cThreads + j*k + l] = thread_data[i*cThreads+j].K->data[n*k + l];
                    allKindex.data[n * k * cThreads + j * k + l] = thread_data[i * cThreads + j].Kindex->data[n * k + l];
                }
            }
        }

        for (int n = 0; n < addRows; n++) {
            quickSelect(allK.data + n * k * cThreads, allKindex.data + n * k * cThreads, 0, k * cThreads - 1, k, K->data + (rows + n) * k, Kindex->data + (rows + n) * k);
        }

        free(allK.data);
        free(allKindex.data);
        rows += addRows;
    }

    for(int i=0; i<qThreads*cThreads; i++){
        free(thread_data[i].C);
        free(thread_data[i].Q);
        free(thread_data[i].K->data);
        free(thread_data[i].K);
        free(thread_data[i].Kindex->data);
        free(thread_data[i].Kindex);
    }
}