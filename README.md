# Knn Search Implementation

| V0-OpenMP      | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  100%   |   ---   |
| Queries/second |  273    |   ---   |

| V0-PTHREADS    | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  100%   |   ---   |
| Queries/second |  448    |   ---   |

| V1-OpenMP      | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  4.02%  |  3.07%  |
| Queries/second |  507    |   383   |

| V1-PTHREADS    | MNIST   | SIFT    |
|----------------|---------|---------|
| Recall         |  4.02%  |  3.07%  |
| Queries/second |  530    |   388   |
