#ifndef FUNCTIONS_CPP_H
#define FUNCTIONS_CPP_H

#include <stddef.h>
#include <vector>
#include "vptree.h"

void searchVPTree(VPNode* node, double* query, int dim, std::vector<double>& distances, double threshold);

#endif 