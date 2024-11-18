#include <queue>
#include <ostream>
#include <iostream>

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
