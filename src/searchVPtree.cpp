#include <../include/functions_cpp.h>

#include <vector>
#include <cmath>

void searchVPTree(VPNode* node, double* query, int dim, std::vector<double>& distances, double threshold) {

    if (node == NULL) return;

    //Calculate distance between query and node->point
    double distance = 0.0;
    for (int i = 0; i < dim; i++) {
        distance += (query[i] - node->point[i]) * (query[i] - node->point[i]);
    }
    distance = sqrt(distance);

    double upperBound = node->radius + threshold;
    double lowerBound = node->radius - threshold < 0 ? 0.5 : node->radius - threshold;

    if(node->radius == 0.0){
        distances.push_back(distance);
        return;
    }

    if(upperBound < distance){
        distances.push_back(distance);
        //Go to the right child
        searchVPTree(node->right, query, dim, distances, threshold);
    }
    else if(lowerBound > distance){
        distances.push_back(distance);
        //Go to the left child
        searchVPTree(node->left, query, dim, distances, threshold);
    }
    else {
        distances.push_back(distance);
        //Go to the left child
        searchVPTree(node->left, query, dim, distances, threshold);
        //Go to the right child
        searchVPTree(node->right, query, dim, distances, threshold);
    }
}