#ifndef VPTREE_H
#define VPTREE_H

typedef struct VPNode {
    int index;
    double* point;  // Vantage point      
    double radius;  // Median        
    struct VPNode* left;
    struct VPNode* right;
} VPNode;


#endif // VPTREE_H
