sway
====

C++ Data Structures (min-max heap, bounded priority queue)

This project contains:
 - a min-max heap implementation (similar interface to the STL max heap)
 - a bounded-priority queue implementation
 - an utility class to parse configuration strings or configuration files

The min-max heap implementation makes use of the fast integer log-2 routine available here:
http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup

The configuration utility class depends on the Boost library.
Unit tests are available. They also depend on the Boost Library.
