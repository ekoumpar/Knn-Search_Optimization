#include <../include/functions_c.h>
#include <../include/functions_cpp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matio.h"
#include <vector>
#include <iostream>
#include <queue>
#include <cstdio>
#include <pthread.h>
#include <cmath>

double thresholdFunc(double x) {
    return 0.856 / pow(x, 0.233);
}

// Define a struct to pass arguments to the thread function
struct ThreadArgs {
    size_t start;
    size_t end;
    const Matrix* Q;
    const VPNode* Corpus;
    size_t k;
    double threshold;
    std::vector<std::vector<Neighbor>>* allNeighbors;
};

// Thread function to process a group of queries
void* processQueries(void* args) {
    ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
    for (size_t i = threadArgs->start; i < threadArgs->end; i++) {
        std::priority_queue<Neighbor, std::vector<Neighbor>, Compare> pq;
        searchVPTree(const_cast<VPNode*>(threadArgs->Corpus), threadArgs->Q->data + i * threadArgs->Q->cols, (int)threadArgs->Q->cols, (int)threadArgs->k, pq, threadArgs->threshold);
        std::vector<Neighbor> neighbors;
        while (!pq.empty()) {
            neighbors.push_back(pq.top());
            pq.pop();
        }
        (*threadArgs->allNeighbors)[i] = std::move(neighbors);
    }
    return nullptr;
}

int main(){
    srand(time(NULL));
    
    Matrix C, Q, K, Kindex;
    size_t k;
    mat_t* wFile = NULL;

    // Read .mat file
    const char* filename = "../mat/large.mat";
    mat_t *file = NULL;
    OpenFile(&file, filename);
    readMatrix(&C, "CORPUS", file);
    readMatrix(&Q, "QUERY", file);

    printf("Enter the number of nearest neighbours: ");
    if (scanf("%zu", &k) != 1) {
        printf("Invalid input for 'k'.\n");
        return -1;
    }

    size_t corpus = C.rows;

    // Mean distance calculation
    double totalDistance = 0.0;
    int totalNodes = 0;

    // Start timer
    double start_time = omp_get_wtime();
   
    int* kindex = (int*)malloc(sizeof(int) * corpus);
    if (kindex == NULL) {
        printf("Memory allocation failed for kindex.\n");
        return -1;
    }
    for (int i = 0; i < corpus; i++) {
        kindex[i] = i + 1;
    }

    VPNode* Corpus = NULL;
    buildVPTree(&C, &Corpus, &totalDistance, &totalNodes, &kindex);

    // Search for k nearest neighbours
    double threshold = (totalDistance / totalNodes) * thresholdFunc(corpus);
    std::vector<std::vector<Neighbor>> allNeighbors(Q.rows); // structure with k nearest neighbours and indexes

    // Number of threads
    const size_t numThreads = 4;
    pthread_t threads[numThreads];
    ThreadArgs threadArgs[numThreads];

    // Divide the queries into groups
    size_t queriesPerThread = Q.rows / numThreads;
    for (size_t t = 0; t < numThreads; t++) {
        threadArgs[t].start = t * queriesPerThread;
        threadArgs[t].end = (t == numThreads - 1) ? Q.rows : (t + 1) * queriesPerThread;
        threadArgs[t].Q = &Q;
        threadArgs[t].Corpus = Corpus;
        threadArgs[t].k = k;
        threadArgs[t].threshold = threshold;
        threadArgs[t].allNeighbors = &allNeighbors;
        pthread_create(&threads[t], nullptr, processQueries, &threadArgs[t]);
    }

    // Join the threads
    for (size_t t = 0; t < numThreads; t++) {
        pthread_join(threads[t], nullptr);
    }

    // End timer
    double end_time = omp_get_wtime();
    
    createMatrix(&K, Q.rows, k);
    createMatrix(&Kindex, Q.rows, k);
    // Printing output
    for (size_t i = 0; i < allNeighbors.size(); i++) {
        std::cout << "Query " << i << ": " << std::endl;
        size_t j = 0;
        for (const auto& neighbor : allNeighbors[i]) {
            std::cout << "  Index: " << neighbor.index << ", Distance: " << neighbor.distance << std::endl;
            K.data[i * K.cols + j] = neighbor.distance;
            Kindex.data[i * K.cols + j] = static_cast<double>(neighbor.index);
            j++;
        }
    }

    printf("Threshold: %f\n", threshold);
    printf("Time taken for calculate k neighbours: %f seconds\n", end_time - start_time);

    // Save results in output.mat
    CreateFile(&wFile);
    saveMatrix(&wFile, K.rows, K.cols, K.data, "K");
    saveMatrix(&wFile, Kindex.rows, Kindex.cols, Kindex.data, "Kindex");
    CloseFile(&wFile);

    free(Q.data);
    free(K.data);
    freeVPTree(&Corpus);
    
    // Close .mat file
    CloseFile(&file);

    return 0;
}