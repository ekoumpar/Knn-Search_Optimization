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

int main(){
    srand(time(NULL));
    
    Matrix C,Q,K,Kindex;
    size_t k;
    mat_t* wFile = NULL;
    
    //const char* filename="../hdf5/mnist-784-euclidean.hdf5";

    /*
    //Read .hdf5 file
    if(load_hdf5(filename, "/train", &C) == -1){
        printf("Error loading matrix C\n");
        return -1;
    }
    if(load_hdf5(filename, "/test", &Q) == -1){
        printf("Error loading matrix Q\n");
        return -1;
    }
    */

    //Read .mat file
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

    //Mean distance calculation
    double totalDistance = 0.0;
    int totalNodes = 0;

    //Start timer
    double start_time = omp_get_wtime();
   
    int* kindex = (int*)malloc(sizeof(int) * C.rows);
    for (int i = 0; i < C.rows; i++) {
        kindex[i] = i + 1;
    }

    VPNode* Corpus = NULL;
    buildVPTree(&C, &Corpus, &totalDistance, &totalNodes,&kindex);

    //Search for k nearest neighbours
    double threshold = (totalDistance / totalNodes)*0.1 ;
    std::vector<std::vector<Neighbor>> allNeighbors(Q.rows); // structure with k nearest neighbours and indexes

    #pragma omp parallel for
        for (size_t i = 0; i < Q.rows; i++){
            using namespace std;
            priority_queue<Neighbor, vector<Neighbor>, Compare> pq;
            searchVPTree(Corpus, Q.data + i * Q.cols, (int)Q.cols, (int)k, pq, threshold);
            std::vector<Neighbor> neighbors;
            while (!pq.empty()) {
            neighbors.push_back(pq.top());
            pq.pop();
            }
            allNeighbors[i] = std::move(neighbors);
        }

    //End timer
    double end_time = omp_get_wtime();
    
    createMatrix(&K, Q.rows, k);
    createMatrix(&Kindex, Q.rows, k);
    //printing output
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

    //save results in output.mat
    createFile(&wFile);
    saveMatrix(&wFile, K.rows, K.cols, K.data, "K");
    saveMatrix(&wFile, Kindex.rows, Kindex.cols, Kindex.data, "Kindex");
    CloseFile(&wFile);

    free(Q.data);
    free(K.data);
    freeVPTree(&Corpus);
    
    //Close .mat file
    CloseFile(&file);

    return 0;
}
