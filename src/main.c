#include <../include/knn.h>

#include <time.h>
#include <omp.h>
#include "matio.h"

int main(){
    srand(time(NULL));
    
    Matrix C, Q, K, Kindex;
    size_t k;
    const char* filename="../mat/large.mat";
    mat_t *file = NULL;
    mat_t *wFile = NULL;

    OpenFile(&file, filename);

    readMatrix(&C, "CORPUS", file);
    readMatrix(&Q, "QUERY", file);

    
    printf("Enter the number of nearest neighbours: ");
    if (scanf("%zu", &k) != 1) {
        printf("Invalid input for 'k'.\n");
        return -1;
    }
    
    //Calculate k nearest neighbours
    createMatrix(&K, Q.rows, k);
    createMatrix(&Kindex, Q.rows, k);
    double start_time = omp_get_wtime();
    knnSearch(&C, &Q, k, &K, &Kindex);
    double end_time = omp_get_wtime();

    printf("---------------K neighbours----------------");
    printMatrix(&K);
    printf("----------------Indexes--------------------");
    printMatrix(&Kindex);

    printf("Time taken for knnSearch: %f seconds\n", end_time - start_time);
    

    createFile(&wFile);
    saveMatrix(&wFile, K.rows, K.cols, K.data, "K");
    saveMatrix(&wFile, Kindex.rows, Kindex.cols, Kindex.data, "Kindex");
    CloseFile(&wFile);



    free(K.data);
    free(Kindex.data);
    free(C.data);
    free(Q.data);
    
    CloseFile(&file);
    return 0;
}
