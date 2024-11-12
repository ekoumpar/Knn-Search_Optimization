#include <../include/knn.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int index;
    Matrix* C;
    Matrix* Q;
    Matrix* K;
    int k;
} ThreadData;

void* knnSearchThread(void* arg){
    ThreadData* thread = (ThreadData*)arg;

    Matrix D;
    createMatrix(&D, thread->Q->rows, thread->C->rows);

    //Calculate the distance between each point in Q and C
    distanceBlas(thread->C, thread->Q, &D);

    //Find the k nearest neighbors
    int drows = (int)D.rows;
    int dCols = (int)D.cols;
    for (int i = 0; i < drows; i++) {
        quickSelect(D.data + i*dCols, 0, dCols - 1, thread->k, thread->K->data + i*thread->k);
    }

    free(D.data);

    pthread_exit(NULL);
}

void knnSearch(const Matrix* C, const Matrix* Q, int k, Matrix* K){

    int N = (int)C->rows;
    int M = (int)Q->rows;
    int dim = (int)C->cols;

    
    int qThreads = 5;
    int cThreads = 5;
    int rows_per_thread_Q = M / qThreads;
    int rows_per_thread_C = N / cThreads;

    pthread_t threads[qThreads*cThreads];
    ThreadData thread_data[qThreads*cThreads];

    for(int i=0; i < qThreads; i++){
        for(int j=0; j<cThreads; j++){

            thread_data[i*cThreads+j].index = i*cThreads+j;

            //Initialize matrix c for each tread
            thread_data[i*cThreads+j].C = (Matrix*)malloc(sizeof(Matrix));
            thread_data[i*cThreads+j].C->data = C->data + j * rows_per_thread_C * dim;
            thread_data[i*cThreads+j].C->rows = (j == cThreads - 1) ? rows_per_thread_C + (N%cThreads) : rows_per_thread_C;
            thread_data[i*cThreads+j].C->cols = dim;

            //Initialize matrix q for each tread
            thread_data[i*cThreads+j].Q = (Matrix*)malloc(sizeof(Matrix));
            thread_data[i*cThreads+j].Q->data = Q->data + i * rows_per_thread_Q * dim;
            thread_data[i*cThreads+j].Q->rows = (i == qThreads - 1) ? rows_per_thread_Q + (M%qThreads) : rows_per_thread_Q;
            thread_data[i*cThreads+j].Q->cols = dim;

            //Initialize matrix k for each tread
            thread_data[i*cThreads+j].K = (Matrix*)malloc(sizeof(Matrix));
            createMatrix(thread_data[i*cThreads+j].K, thread_data[i*cThreads+j].Q->rows, k);

            thread_data[i*cThreads+j].k = k;

            pthread_create(&threads[i*cThreads+j], NULL, knnSearchThread, (void*)&thread_data[i*cThreads+j]);
        }
    }

    int rows =0;
    for (int i = 0; i < qThreads; i++) {
    
        int addRows = (int)thread_data[i*cThreads].K->rows;
        Matrix allK;
        createMatrix(&allK, addRows, k*cThreads);

        for (int j = 0; j < cThreads; j++) {
            pthread_join(threads[i*cThreads+j], NULL);
            for(int n=0; n<addRows; n++){
                for(int l=0; l<k; l++){
                    allK.data[n*k*cThreads + j*k + l] = thread_data[i*cThreads+j].K->data[n*k + l];
                }
            }
        }

        for(int n=0; n<addRows; n++){
            quickSelect(allK.data + n*k*cThreads, 0, k*cThreads - 1, k, K->data + (rows + n)*k);
        }

        free(allK.data);
        rows += addRows;
    }

    for(int i=0; i<qThreads*cThreads; i++){
        free(thread_data[i].C);
        free(thread_data[i].Q);
        free(thread_data[i].K->data);
        free(thread_data[i].K);
    }
}