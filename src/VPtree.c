#include <../include/knn.h>

#include <stdio.h>


void buildVPTree(Matrix* matrix, VPNode** node) {

    int dim =(int) matrix->cols;
    int N = (int)matrix->rows;

    if (N == 0) {
        (*node) = NULL;
        free(matrix->data);
        return;
    }

    //initialize first point as Vantage point
    *node = (VPNode*)malloc(sizeof(VPNode));
    if (*node == NULL) {
        printf("Problem with memory allocation in buildVPTree\n");
        return;
    }
    (*node)->point = (double*)malloc(dim * sizeof(double));
    if ((*node)->point == NULL) {
        printf("Problem with memory allocation in buildVPTree\n");
        free(*node);
        return;
    }
    for (int i = 0; i < dim; i++) {
        (*node)->point[i] = matrix->data[i];
    }

    //case of a leaf
    if (N == 1) {
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->radius = 0;
        free(matrix->data);
        return;
    }
    
    //distanceBlas
    Matrix Vpoint;
    createArray(&Vpoint, 1, dim);
    for (int i = 0; i < dim; i++) {
        Vpoint.data[i] = (*node)->point[i];
    }
    
    Matrix distances;
    createArray(&distances, N - 1, 1);

    
    //ignore first point of array
    double* data_Matrix = matrix->data;
    if (matrix->data != NULL) {
        matrix->data += dim;
        matrix->rows = N - 1;
    }

    distanceBlas(matrix, &Vpoint, &distances);
    free(Vpoint.data);

    //calculate median of distances
    double median= quickMedian(distances.data, 0, N - 2);

    //for debugging
    printf("distances %d: ", N);
    for (int i = 0; i < N - 1; i++) {
        printf("%f  ", distances.data[i]);
    }
    printf("  median :%f", median);


    //seperate between left and right nodes
    Matrix Left, Right;
    createArray(&Left, N - 1, dim);
    createArray(&Right, N - 1, dim);
    

    int left_count = 0, right_count = 0;
    for (int i = 0; i < (N-1) ; i++) {
        if (distances.data[i] <= median) {
            for (int j = 0; j < dim; j++) {
                Left.data[left_count * dim + j] = matrix->data[i * dim + j];
            }
            left_count++;
        }
        else {
            for (int j = 0; j < dim; j++) {
                Right.data[right_count * dim + j] = matrix->data[i * dim + j]; 
            }
            right_count++;
        }
    }
    Left.rows = left_count;
    Right.rows = right_count;

    //debugging
    printf("\n");

    free(distances.data);
    free(data_Matrix);

    //node initiallize
    (*node)->radius = median;
    (*node)->left = NULL;
    (*node)->right = NULL;

    //copy of data pointers for deallocation

    
    buildVPTree(&Left, &(*node)->left);
    buildVPTree(&Right, &(*node)->right);


    return;
    
}


void freeVPTree(VPNode** node) {

	if (*node == NULL) return;

	freeVPTree(&(*node)->left);
	freeVPTree(&(*node)->right);

	free((*node)->point);
	free(*node);
    *node = NULL;
}

void printVPTree( VPNode* node, int level, int dim) {
	if (node == NULL) return;

	// levels of tree as tabs
	for (int i = 0; i < level; i++) {
		printf("    ");
	}

	// print point and radius of node 
	printf("Point: (");
	for (int i = 0; i < dim; i++) {
		printf("%.2f", node->point[i]);
		if (i < dim - 1) printf(", ");
	}
	printf("), Radius: %f\n", node->radius);
     
   
	printVPTree(node->left, level + 1, dim);
	printVPTree(node->right, level + 1, dim);
}
