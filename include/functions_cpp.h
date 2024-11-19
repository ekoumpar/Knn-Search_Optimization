#ifndef FUNCTIONS_CPP_H
#define FUNCTIONS_CPP_H

#include <queue>
#include <ostream>
#include <iostream>
#include "vptree.h"

using namespace std;
struct Neighbor {
    double distance;
    int index;
};
struct Compare {
    bool operator()(Neighbor const& p1, Neighbor const& p2) const {
        return p1.distance < p2.distance; 
    }
};
void searchVPTree(VPNode* node, double* query, int dim,int k, priority_queue<Neighbor,vector<Neighbor>,Compare>& pq, double threshold);


#endif 
