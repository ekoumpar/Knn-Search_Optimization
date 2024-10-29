#include <../include/knn.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    
    Matrix C, Q, D, K;
    size_t corpus, query, dimensions, k;
    
    printf("Enter the number of points in corpus-set: ");
    scanf("%zu",  &corpus);
    printf("Enter the number of points in query-set: ");
    scanf("%zu", &query);
    printf("Enter the dimensions: ");
    scanf("%zu", &dimensions);
    printf("Enter the number of nearest neighbours: ");
    scanf("%zu", &k);
    
    randomMatrix(&C, corpus, dimensions);
    randomMatrix(&Q, query, dimensions);
    createMatrix(&D, query, corpus);
    createMatrix(&K, query, k);

    printf("---------------Corpus-Set----------------");
    printMatrix(&C);
    printf("---------------Query-Set-----------------");
    printMatrix(&Q);

    //Calculate Distances
    distanceBlas(&C, &Q, &D);
    printf("---------------Distances-----------------");
    printMatrix(&D);

    //Sorting of kth elements
    int rows = (int)D.rows;
    int dCols = (int)D.cols;
    int kCols = (int)K.cols; 
    for (int i = 0; i < rows; i++) {
        quickSelect(D.data + i*dCols, 0, dCols - 1, kCols, K.data + i*kCols);
    }

    printf("---------------K neighbours----------------");
    printMatrix(&K);

    free(K.data);
    free(C.data);
    free(Q.data);
    free(D.data);

    return 0;
}
