// (C) Copyright Andrea Sansottera 2011
// Based on the research paper:
//   Min-Max Heaps and Generalized Priority Queues
//   M. D. Atkinson, J. R. Sack, N. Santoro and T. Strothotte
//   Communications of the ACM, October 1986

#ifndef MIN_MAX_HEAP_H
#define MIN_MAX_HEAP_H

#include <algorithm>
#include <cstdint>

namespace sway {

template<class T>
T ilog2(T x);

template<>
inline uint32_t ilog2(uint32_t x) {
    register uint32_t l = 0;
    if(x >= 1<<16) { x >>= 16; l |= 16; }
    if(x >= 1<<8) { x >>= 8; l |= 8; }
    if(x >= 1<<4) { x >>= 4; l |= 4; }
    if(x >= 1<<2) { x >>= 2; l |= 2; }
    if(x >= 1<<1) l |= 1;
    return l;
}

template<>
inline uint64_t ilog2(uint64_t x) {
	register uint64_t l = 0;
	if (x >= static_cast<uint64_t>(1) << 32) { x >>= 32; l |= 32; }
	if (x >= static_cast<uint64_t>(1) << 16) { x >>= 16; l |= 16; }
	if (x >= static_cast<uint64_t>(1) << 8) { x >>= 8; l |= 8; }
	if (x >= static_cast<uint64_t>(1) << 4) { x >>= 4; l |= 4; }
	if (x >= static_cast<uint64_t>(1) << 2) { x >>= 2; l |= 2; }
	if (x >= static_cast<uint64_t>(1) << 1) l |= 1; 
	return l;
}

template<class RAI>
RAI getLeftChild(RAI first, RAI last, RAI i) {
	int offset = (i - first) * 2 + 1;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
RAI getRightChild(RAI first, RAI last, RAI i) {
	int offset = (i - first) * 2 + 2;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
int getLevel(RAI first, RAI last, RAI i) {
	// the difference cannot be negative
	std::size_t diff = static_cast<std::size_t>(i - first);
	return ilog2(diff+1);
}

template<class RAI>
RAI getParent(RAI first, RAI last, RAI i) {
	int diff = i - first;
	if (diff < 1) {
		return last;
	}
	return first + (diff-1)/2;
}

template<class RAI>
RAI getGrandParent(RAI first, RAI last, RAI i) {
	int diff = i - first;
	if (diff < 3) {
		return last;
	}
	return first + (diff-3)/4;
}

template<class RAI>
RAI getSmallestChildOrGrandchild(RAI first, RAI last, RAI i) {
	/* If there are no children, return last */
	RAI smallest = last;
	RAI left = getLeftChild(first, last, i);
	if (left < last) {
		smallest = left;
		RAI leftLeft = getLeftChild(first, last, left);
		RAI leftRight = getRightChild(first, last, left);
		if (leftLeft < last && *leftLeft < *smallest) {
			smallest = leftLeft;
		}
		if (leftRight < last && *leftRight < *smallest) {
			smallest = leftRight;
		}
	}
	RAI right = getRightChild(first, last, i);
	if (right < last) {
		if (*right < *smallest) {
			smallest = right;
		}
		RAI rightLeft = getLeftChild(first, last, right);
		RAI rightRight = getRightChild(first, last, right);
		if (rightLeft < last && *rightLeft < *smallest) {
			smallest = rightLeft;
		}
		if (rightRight < last && *rightRight < *smallest) {
			smallest = rightRight;
		}
	}
	return smallest;
}

template<class RAI, class Compare>
RAI getSmallestChildOrGrandchild(RAI first, RAI last, RAI i, Compare comp) {
	/* If there are no children, return last */
	RAI smallest = last;
	RAI left = getLeftChild(first, last, i);
	if (left < last) {
		smallest = left;
		RAI leftLeft = getLeftChild(first, last, left);
		RAI leftRight = getRightChild(first, last, left);
		if (leftLeft < last && comp(*leftLeft, *smallest)) {
			smallest = leftLeft;
		}
		if (leftRight < last && comp(*leftRight, *smallest)) {
			smallest = leftRight;
		}
	}
	RAI right = getRightChild(first, last, i);
	if (right < last) {
		if (comp(*right, *smallest)) {
			smallest = right;
		}
		RAI rightLeft = getLeftChild(first, last, right);
		RAI rightRight = getRightChild(first, last, right);
		if (rightLeft < last && comp(*rightLeft, *smallest)) {
			smallest = rightLeft;
		}
		if (rightRight < last && comp(*rightRight, *smallest)) {
			smallest = rightRight;
		}
	}
	return smallest;
}

template<class RAI>
RAI getLargestChildOrGrandchild(RAI first, RAI last, RAI i) {
	/* If there are no children return last */
	RAI largest = last;
	RAI left = getLeftChild(first, last, i);
	if (left < last) {
		largest = left;
		RAI leftLeft = getLeftChild(first, last, left);
		RAI leftRight = getRightChild(first, last, left);
		if (leftLeft < last && *leftLeft > *largest) {
			largest = leftLeft;
		}
		if (leftRight < last && *leftRight > *largest) {
			largest = leftRight;
		}
	}
	RAI right = getRightChild(first, last, i);
	if (right < last) {
		if (*right > *largest) {
			largest = right;
		}
		RAI rightLeft = getLeftChild(first, last, right);
		RAI rightRight = getRightChild(first, last, right);
		if (rightLeft < last && *rightLeft > *largest) {
			largest = rightLeft;
		}
		if (rightRight < last && *rightRight > *largest) {
			largest = rightRight;
		}
	}
	return largest;
}

template<class RAI, class Compare>
RAI getLargestChildOrGrandchild(RAI first, RAI last, RAI i, Compare comp) {
	/* If there are no children return last */
	RAI largest = last;
	RAI left = getLeftChild(first, last, i);
	if (left < last) {
		largest = left;
		RAI leftLeft = getLeftChild(first, last, left);
		RAI leftRight = getRightChild(first, last, left);
		if (leftLeft < last && comp(*largest, *leftLeft)) {
			largest = leftLeft;
		}
		if (leftRight < last && comp(*largest, *leftRight)) {
			largest = leftRight;
		}
	}
	RAI right = getRightChild(first, last, i);
	if (right < last) {
		if (comp(*largest, *right)) {
			largest = right;
		}
		RAI rightLeft = getLeftChild(first, last, right);
		RAI rightRight = getRightChild(first, last, right);
		if (rightLeft < last && comp(*largest, *rightLeft)) {
			largest = rightLeft;
		}
		if (rightRight < last && comp(*largest, *rightRight)) {
			largest = rightRight;
		}
	}
	return largest;
}

template<class RAI>
void trickleDownMin(RAI first, RAI last, RAI i) {
	if (getLeftChild(first, last, i) < last) {
		RAI m = getSmallestChildOrGrandchild(first, last, i);
		if (getGrandParent(first, last, m) == i) {
			if (*m < *i) {
				std::iter_swap(m, i);
				RAI parent = getParent(first, last, m);
				if (*m > *parent) {
					std::iter_swap(m, parent);
				}
				trickleDownMin(first, last, m);
			}
		} else {
			if (*m < *i) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI, class Compare>
void trickleDownMin(RAI first, RAI last, RAI i, Compare comp) {
	if (getLeftChild(first, last, i) < last) {
		RAI m = getSmallestChildOrGrandchild(first, last, i, comp);
		if (getGrandParent(first, last, m) == i) {
			if (comp(*m, *i)) {
				std::iter_swap(m, i);
				RAI parent = getParent(first, last, m);
				if (comp(*parent, *m)) {
					std::iter_swap(m, parent);
				}
				trickleDownMin(first, last, m, comp);
			}
		} else {
			if (comp(*m, *i)) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI>
void trickleDownMax(RAI first, RAI last, RAI i) {
	if (getLeftChild(first, last,i) < last) {
		RAI m = getLargestChildOrGrandchild(first, last, i);
		if (getGrandParent(first, last, m) == i) {
			if (*m > *i) {
				std::iter_swap(m, i);
				RAI parent = getParent(first, last, m);
				if (*m < *parent) {
					std::iter_swap(m, parent);
				}
				trickleDownMax(first, last, m);
			}
		} else {
			if (*m > *i) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI, class Compare>
void trickleDownMax(RAI first, RAI last, RAI i, Compare comp) {
	if (getLeftChild(first, last,i) < last) {
		RAI m = getLargestChildOrGrandchild(first, last, i, comp);
		if (getGrandParent(first, last, m) == i) {
			if (comp(*i, *m)) {
				std::iter_swap(m, i);
				RAI parent = getParent(first, last, m);
				if (comp(*m, *parent)) {
					std::iter_swap(m, parent);
				}
				trickleDownMax(first, last, m, comp);
			}
		} else {
			if (comp(*i, *m)) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI>
void trickleDown(RAI first, RAI last, RAI i) {
	if (getLevel(first, last, i) % 2 == 0) {
		trickleDownMin(first, last, i);
	} else {
		trickleDownMax(first, last, i);
	}
}

template<class RAI, class Compare>
void trickleDown(RAI first, RAI last, RAI i, Compare comp) {
	if (getLevel(first, last, i) % 2 == 0) {
		trickleDownMin(first, last, i, comp);
	} else {
		trickleDownMax(first, last, i, comp);
	}
}

template<class RAI>
void bubbleUpMin(RAI first, RAI last, RAI i) {
	RAI gp = getGrandParent(first, last, i);
	if (gp != last && *i < *gp) {
		std::iter_swap(i, gp);
		bubbleUpMin(first, last, gp);
	}
}

template<class RAI, class Compare>
void bubbleUpMin(RAI first, RAI last, RAI i, Compare comp) {
	RAI gp = getGrandParent(first, last, i);
	if (gp != last && comp(*i, *gp)) {
		std::iter_swap(i, gp);
		bubbleUpMin(first, last, gp, comp);
	}
}

template<class RAI>
void bubbleUpMax(RAI first, RAI last, RAI i) {
	RAI gp = getGrandParent(first, last, i);
	if (gp != last && *i > *gp) {
		std::iter_swap(i, gp);
		bubbleUpMax(first, last, gp);
	}
}

template<class RAI, class Compare>
void bubbleUpMax(RAI first, RAI last, RAI i, Compare comp) {
	RAI gp = getGrandParent(first, last, i);
	if (gp != last && comp(*gp, *i)) {
		std::iter_swap(i, gp);
		bubbleUpMax(first, last, gp, comp);
	}
}


template<class RAI>
void bubbleUp(RAI first, RAI last, RAI i) {
	RAI parent = getParent(first, last, i);
	if (getLevel(first, last, i) % 2 == 0) {
		if (parent != last && *i > *parent) {
			std::iter_swap(i, parent);
			bubbleUpMax(first, last, parent);
		} else {
			bubbleUpMin(first, last, i);
		}
	} else {
		if (parent != last && *i < *parent) {
			std::iter_swap(i, parent);
			bubbleUpMin(first, last, parent);
		} else {
			bubbleUpMax(first, last, i);
		}
	}
}

template<class RAI, class Compare>
void bubbleUp(RAI first, RAI last, RAI i, Compare comp) {
	RAI parent = getParent(first, last, i);
	if (getLevel(first, last, i) % 2 == 0) {
		if (parent != last && comp(*parent, *i)) {
			std::iter_swap(i, parent);
			bubbleUpMax(first, last, parent, comp);
		} else {
			bubbleUpMin(first, last, i, comp);
		}
	} else {
		if (parent != last && comp(*i, *parent)) {
			std::iter_swap(i, parent);
			bubbleUpMin(first, last, parent, comp);
		} else {
			bubbleUpMax(first, last, i, comp);
		}
	}
}

template<class RAI>
void make_minmaxheap(RAI first, RAI last) {
	if (last - first >= 2) {
		int offset = (last - first) / 2 - 1;
		for (RAI i = first + offset; i > first; --i) {
			trickleDown(first, last, i);
		}
		trickleDown(first, last, first);
	}
}

template<class RAI, class Compare>
void make_minmaxheap(RAI first, RAI last, Compare comp) {
	if (last - first >= 2) {
		int offset = (last - first) / 2 - 1;
		for (RAI i = first + offset; i > first; --i) {
			trickleDown(first, last, i, comp);
		}
		trickleDown(first, last, first, comp);
	}
}

template<class RAI>
void popmin_minmaxheap(RAI first, RAI last) {
	std::iter_swap(first, last-1);
	trickleDown(first, last-1, first);
}

template<class RAI, class Compare>
void popmin_minmaxheap(RAI first, RAI last, Compare comp) {
	std::iter_swap(first, last-1);
	trickleDown(first, last-1, first, comp);
}

template<class RAI>
void popmax_minmaxheap(RAI first, RAI last) {
	if (last-first < 2) {
		return;
	}
	if (last-first == 2 || *(first+1) > *(first+2)) {
		std::iter_swap(first+1, last-1);
		trickleDown(first, last-1, first+1);
	} else {
		std::iter_swap(first+2, last-1);
		trickleDown(first, last-1, first+2);
	}
}

template<class RAI, class Compare>
void popmax_minmaxheap(RAI first, RAI last, Compare comp) {
	if (last-first < 2) {
		return;
	}
	if (last-first == 2 || comp(*(first+2), *(first+1))) {
		std::iter_swap(first+1, last-1);
		trickleDown(first, last-1, first+1, comp);
	} else {
		std::iter_swap(first+2, last-1);
		trickleDown(first, last-1, first+2, comp);
	}
}

template<class RAI>
void push_minmaxheap(RAI first, RAI last) {
	bubbleUp(first, last, last-1);
}

template<class RAI, class Compare>
void push_minmaxheap(RAI first, RAI last, Compare comp) {
	bubbleUp(first, last, last-1, comp);
}

}

#endif
