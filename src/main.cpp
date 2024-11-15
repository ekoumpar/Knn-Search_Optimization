#include <../include/functions_c.h>
#include <../include/functions_cpp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matio.h"
#include <vector>

int main(){
    srand(time(NULL));
    
    Matrix C, Q, K;
    size_t k;
    const char* filename="../hdf5/mnist-784-euclidean.hdf5";

    //Read .hdf5 file
    if(load_hdf5(filename, "/train", &C) == -1){
        printf("Error loading matrix C\n");
        return -1;
    }
    if(load_hdf5(filename, "/test", &Q) == -1){
        printf("Error loading matrix Q\n");
        return -1;
    }

    //Read .mat file
    // mat_t *file = NULL;
    // OpenFile(&file, filename);
    // readMatrix(&C, "CORPUS", file);
    // readMatrix(&Q, "QUERY", file);

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

    //after bulidVPTree C is deleted!
    VPNode* Corpus = NULL;
    buildVPTree(&C, &Corpus, &totalDistance, &totalNodes);

    //Search for k nearest neighbours
    createMatrix(&K, Q.rows, k);
    double threshold = (totalDistance / totalNodes) * 0.1;

    #pragma omp parallel for
    for(size_t i = 0; i < Q.rows; i++){
        std::vector<double> distances;
        searchVPTree(Corpus, Q.data + i*Q.cols, (int)Q.cols, distances, threshold);
        quickSelect(distances.data(), 0, distances.size()-1, k, K.data + i*k);
    }

    //End timer
    double end_time = omp_get_wtime();

    //Print k nearest neighbours
    printf("---------------%zu-Nearest-Neighbours-----------------", k);
    printMatrix(&K);

    printf("Threshold: %f\n", threshold);
    printf("Time taken for calculate k neighbours: %f seconds\n", end_time - start_time);
    
    free(Q.data);
    free(K.data);
    freeVPTree(&Corpus);
    
    //Close .mat file
    //CloseFile(&file);

    return 0;
}
