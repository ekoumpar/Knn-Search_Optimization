# Knn Search Implementation
This project aims to compute the K-Nearest Neighbors from a QUERY dataset to a CORPUS dataset, known as Knn-Search Problem.
## Description
There are two implementations of the KNN-Search algorithm:
- [Version 0](V0) for an Accurate Approach
- [Version 1](V1) for an Approximate Approach

Each version is implemented in OpenMP, OpenCilk, PTHREADS.

Every implementation has:
- **src** folder contains implementation of functions
- **include** folder contains the required header files
- **mat** folder contains data sets for testing in `.mat` format
- **outputs** folder contains results from running the code with MNIST or SIFT datasets

A [Report](knn_search.pdf) is available including more details about the methods used in this project.

## Setup
### Installation steps
1. Clone the repository 
```
git clone https://github.com/username/knn-search.git
cd knn-search
```
2. Install Dependencies: Ensure all required libraries are installed.
3. Adjust MakeFile to ensure libraries paths match.
4. Go to the src directory of the project
5. Execute the Makefile with the command:
```
make
```
5. Run the Implementation:
```
./knn
```
### Supported inputs
- **Datasets from HDF5 Files (Default)**

Turn off section `//read .mat files` in `main.c` or `main.cpp` file with comment format:
```C
/*
//Read .mat file
const char* filename = "../mat/medium.mat";
mat_t *file = NULL;
OpenFile(&file, filename);
readMatrix(&C, "CORPUS", file);
readMatrix(&Q, "QUERY", file);
*/
```
- **Datasets from MatFiles**

By default, the first dataset should be named CORPUS and the second dataset QUERY. 

Turn off the following section in `main.c` or `main.cpp` with comment format:
 ```C
/*
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
*/
```
#### Note:
- Input file path should be added to `const char* filename="../path";` command.


### Output
The matrix of k distances found is printed in terminal and as a deafult a Matfile named `output.mat` is created, in which 
K (for distances) and Kindex (for indexes) matrices are saved.
In order to deactivate the last option, comment format shuld be added to the followng section of `main.c` or `main.cpp`
```C
/*
 // Save results in output.mat
    CreateFile(&wFile);
    saveMatrix(&wFile, K.rows, K.cols, K.data, "K");
    saveMatrix(&wFile, Kindex.rows, Kindex.cols, Kindex.data, "Kindex");
    CloseFile(&wFile);
*/
```

## Testing
Each version of our project has been tested with the pre-existing datasets [MNIST](https://yann.lecun.com/exdb/mnist/) and [SIFT](http://corpus-texmex.irisa.fr/) obtained from 
[ANN-Benchmarks](https://github.com/erikbern/ann-benchmarks.git). 

The efficiency of our algorithm was measured using two key metrics:
- **Recall:** This metric evaluates the accuracy of the algorithm in identifying the correct neighbors. A higher recall indicates better performance in terms of accuracy.
- **Queries per Second (QPS):** This measures the speed of the algorithm, indicating how many queries can be processed per second.
###  V0
<table>
  <tr>
    <td>
      
| V0-OpenMP      | MNIST   | SIFT    |    
|----------------|---------|---------|   
| Recall         |  100%   |   ---   |   
| Queries/second |  273    |   ---   |   
      
</td>
    <td>
      
| V0-PTHREADS    | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  100%   |   ---   |
| Queries/second |  448    |   ---   |
      
   </td>
  </tr>
</table>

### V1

<table>
  <tr>
    <td>

| V1-OpenMP      | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  4.02%  |  3.07%  |
| Queries/second |  507    |   383   |
</td>
    <td>

| V1-PTHREADS    | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  4.02%  |  3.07%  |
| Queries/second |  530    |   388   |
  </td>
  </tr>
</table>

**Note for V1:** Depending on the desired trade-off between accuracy and performance, there is a function that can be adjusted accordingly.
In other words higher accuracy can be achieved at the cost of slower execution.
