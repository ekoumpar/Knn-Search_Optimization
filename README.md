# Knn Search Implementation
This project aims to compute the K-Nearest Neighbors from a QUERY dataset to a CORPUS dataset, known as Knn-Search Problem.
## Description
There are two implementations of the KNN-Search algorithm:
- [Version 0](V0) for an Accurate Approach
- [Version 1](V1) for an Approximate Approach

Each version is implemented in OpenMP, OpenCilk and PTHREADS.

Every implementation includes the following:
- **src** folder contains the source code for functions and algorithms
- **include** folder holds the required header files
- **mat** folder contains data sets for testing in `.mat` format
- **outputs** folder contains outputs files from testing with MNIST or SIFT datasets

A [Report](knn_search.pdf) is available including more details about the methods used in this project.

## Setup
### Installation steps
1. Clone the repository 
```
git clone https://github.com/mariaxarisi/HW1.git -b branch_name
```
Select one **branch_name**  of the following list based on the implementation you want to execute:
- V0-OpenCilk
- V0-OpenMP
- V0-Pthreads
- V1-OpenCilk
- V1-OpenMP
- V1-Pthreads

2. Ensure all libraries that **Makefile** contains are installed.
3. Adjust **MakeFile** to ensure libraries paths match.
4. Modify the following command in **main.c** or **main.cpp** file to include the path to the dataset (in **HDF5** format) for testing:
```C
const char* filename="../path";
```

**Note**: If cloning the repository is not working, follow these steps to download the implementation manually:
1. Navigate to the version you want (e.g., V0 or V1) from the available files.
2. Select the desired implementation (e.g., OpenMP, OpenCilk, or PTHREADS).
3. Click the green **Code** button.
4. Choose **Download ZIP**.
5. Extract the downloaded ZIP file to a local directory.
6. Follow **installation steps 2 and 3**.

### Instructions (Linux or Unix-like Interface)
1. Navigate to the project's directory:
```bash
 cd /path/to/project
```
**Note**: Replace ```/path/to/project``` with the actual path to your project on your system.

2. Go to the **src** folder:
 ```bash
 cd src
```
3. Run the **Makefile**:
```bash
make
```
4. Execute the implementation:
```bash
./knn
```
### Supported inputs
- **Datasets from HDF5 Files (Default)**

"Turn off" the following sections in **main.c** or **main.cpp** file using comment syntax:
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
```C
//CloseFile(&file);
```

- **Datasets from MatFiles**

By default, the first dataset should be named CORPUS and the second dataset QUERY. 

"Turn off" the following section in **main.c** or **main.cpp** using comment syntax:
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
A matrix of k distances found for every query is printed in terminal and as a default a **Matfile** named **output.mat** is created, in which 
K (for distances) and Kindex (for indexes) matrices are saved.
In order to deactivate the last option, comment syntax should be added to the following section of **main.c** or **main.cpp** files:
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

To measure recall we used a function implemented in **MATLAB**, which compares the results between our implementation, saved in **output.mat** and MATLAB's function knnSearch.

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
