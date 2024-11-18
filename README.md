# VP TREE implementation with k nearest neighbours selected from a Max-Priority queue
How It Works
1. Building the VP Tree

The dataset is recursively partitioned as follows:

    A vantage point (VP) is selected from the points.
    The distances of all other points to the VP are calculated.
    The points are split into two subsets:
        Inside Radius: Points within or equal to a threshold distance from the VP (e.g., the median distance).
        Outside Radius: Points farther than the threshold distance.
    This process repeats recursively for each subset, creating a binary tree.

2. k-NN Search

To find the k-nearest neighbors of a query point:

    Traverse the VP Tree and compute the distance between the query point and the vantage point at each node.
    Use a max-priority queue to maintain the k-nearest neighbors found so far, stored as (distance, index) pairs.
        If the queue size exceeds k, the farthest neighbor (maximum distance) is removed.
    Use pruning:
        If the query point lies within the radius of the VP, explore the inside subset first.
        If it lies outside, explore the outside subset.
        Skip branches that cannot contain closer neighbors based on the triangle inequality.
    The priority queue contains the k-nearest neighbors at the end of the traversal.
