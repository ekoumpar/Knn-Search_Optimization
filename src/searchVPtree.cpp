#include <../include/functions_cpp.h>

#include <vector>
#include <cmath>
#include <queue>
#include <cstdio>
using namespace std;

void searchVPTree(VPNode* node, double* query, int dim,int k, priority_queue<Neighbor,vector<Neighbor>,Compare>& pq, double threshold) {

    if (node == NULL) return;

    //Calculate distance between query and node->point
    double distance = 0.0;
    for (int i = 0; i < dim; i++) {
        distance += (query[i] - node->point[i]) * (query[i] - node->point[i]);
    }
    distance = sqrt(distance);
    
    if ((int)pq.size() < k) {
        pq.push(Neighbor{ distance, node->index });
    }
    else if (distance < pq.top().distance) {
        pq.pop();
        pq.push(Neighbor{ distance, node->index });
    }
    //case of a leaf
    if(node->radius == 0.0){
        return;
    }

    double upperBound = node->radius + threshold;
    double lowerBound = node->radius - threshold < 0 ? 0.5: node->radius - threshold;

    if(upperBound < distance){
        //Go to the right child
        searchVPTree(node->right, query, dim, k, pq, threshold);
    }
    else if(lowerBound > distance){
        //Go to the left child
        searchVPTree(node->left, query, dim, k, pq, threshold);
    }
    else {
        //Go to the left child
        searchVPTree(node->left, query, dim, k, pq, threshold);
        //Go to the right child
        searchVPTree(node->right, query, dim, k, pq, threshold);
    }
}
