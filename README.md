# Knn Search Implementation
This project aims to compute the K-Nearest Neighbors from a QUERY dataset to a CORPUS dataset, known as Knn-Search Problem.
## Description
There are two implementations of the KNN-Search algorithm:
- [Version 0](V0) for an Accurate Approach
- [Version 1](V1) for an Approximate Approach

Each version is implemented in OpenMP, OpenCilk, PTHREADS.

A [Report](knn_search) is available including more details about the methods used in this project.

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
- Datasets from HDF5 Files (Default)

Turn off section `//read .mat files` with comment format:
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
- Datasets from MatFiles

Turn off line `const char* filename="../hdf5/mnist-784-euclidean.hdf5";` and section`read hdf5 files` with comment format:
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
Input file path should be added to `const char* filename="...";` command.

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
