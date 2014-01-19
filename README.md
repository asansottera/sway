sway
====

C++ Data Structures (min-max heap, bounded priority queue)

This project contains:
 - a min-max heap implementation (similar interface to the STL max heap)
 - a bounded-priority queue implementation
 - an utility class to parse configuration strings or configuration files

Min-max heaps allow the following operations:
 - construction (make_minmaxheap), complexity O(N)
 - access to minimum element (min_minmaxheap), O(1)
 - access to maximum element (max_minmaxheap), O(1)
 - insertion (push_minmaxheap), complexity O(log N)
 - removal of minimum element (popmin_minmaxheap), complexity O(log N)
 - removal of maximum element (popmax_minmaxheap), complexity O(log N)
 
Reference: <i>Min-Max Heaps and Generalized Priority Queues</i>, M. D. Atkinson, J. R. Sack, N. Santoro and T. Strothotte, Communications of the ACM, October 1986

The min-max heap implementation makes use of the fast integer log-2 routine available here:
http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup

The configuration utility class depends on the Boost library.

Unit tests are available. They also depend on the Boost Library.
