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
    const char* filename="large.mat";
    mat_t *file = NULL;

    OpenFile(&file, filename);
   
    //get Corpus
    readMatrix(&C, "CORPUS", file);

    // //for debugging
    // printf("---------------Corpus-Set----------------");
    // printMatrix(&C);

    readMatrix(&Q, "QUERY", file);
    printf("Enter the number of nearest neighbours: ");
     if (scanf("%zu", &k) != 1) {
          printf("Invalid input for 'k'.\n");
          return -1;
    }
    
    // printf("---------------Query-Set-----------------");
    // printMatrix(&Q);

    size_t corpus = C.rows;
    size_t query = Q.rows;

    //Start timer
    double start_time = omp_get_wtime();

    //after bulidVPTree C is deleted!
    VPNode* Corpus = NULL;
    buildVPTree(&C, &Corpus);

    // //for debugging
    // printf("\n\n");
    // printVPTree(Corpus, 0, Q.cols);
    // printf("\n\n");

    //Search for k nearest neighbours
    createMatrix(&K, Q.rows, k);

    #pragma omp parallel for
    for(size_t i = 0; i < Q.rows; i++){
        std::vector<double> distances;
        searchVPTree(Corpus, Q.data + i*Q.cols, (int)Q.cols, distances, 0.3);
        quickSelect(distances.data(), 0, distances.size()-1, k, K.data + i*k);
    }

    //End timer
    double end_time = omp_get_wtime();

    //Print k nearest neighbours
    printf("---------------%zu-Nearest-Neighbours-----------------", k);
    printMatrix(&K);

    printf("Time taken for calculate k neighbours: %f seconds\n", end_time - start_time);
    
    free(Q.data);
    free(K.data);
    freeVPTree(&Corpus);
    CloseFile(&file);
    return 0;
}
