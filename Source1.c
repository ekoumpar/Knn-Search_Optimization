#include <mat.h>
#include <iostream>


int main() {

    knnsearch("my3Dmatrices.mat", "my3Dmatrices.mat", 5, 2);

	







}

void knnsearch(char** queryfile, char** corpusfile,int k, int dimensions ) {


    //get Q
    while ((Q = readQ(queryfile)) == NULL) {
        printf("Error reading query file!");
        exit(1);
    }
    //get C
    while ((C = readQ(corpusfile)) == NULL) {
        printf("Error reading corpus file!");
        exit(1);
    }



}


//read matrix Q from mat file
mxArray* readQ(char ** filename) {

    MATFile* file;
    mxArray* Q;
    

    //open mat file
    file = matOpen(filename[], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // get query
    Q = matGetVariable(file, "query");
    if (Q == NULL) {
        printf("Query not found\n");
        return NULL;
    }
    return Q;    
}

//read matrix C from mat file
mxArray* readC(char ** filename) {

    MATFile* file;
    mxArray* C;

    //open mat file
    file = matOpen(filename[], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // get corpus
    C = matGetVariable(file, "corpus");
    if (C == NULL) {
        printf("Corpus not found\n");
        return NULL;
    }
    return C;

}






