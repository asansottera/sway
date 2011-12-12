// (C) Copyright Andrea Sansottera 2011
// Based on the research paper:
//   Min-Max Heaps and Generalized Priority Queues
//   M. D. Atkinson, J. R. Sack, N. Santoro and T. Strothotte
//   Communications of the ACM, October 1986

#ifndef SWAY_MIN_MAX_HEAP_H
#define SWAY_MIN_MAX_HEAP_H

#include <algorithm>
#include <sway/ilog2.hpp>

namespace sway {

template<class RAI>
RAI get_left_child(RAI first, RAI last, RAI i) {
	int offset = (i - first) * 2 + 1;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
RAI get_right_child(RAI first, RAI last, RAI i) {
	int offset = (i - first) * 2 + 2;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
int get_level(RAI first, RAI last, RAI i) {
	// the difference cannot be negative
	std::size_t diff = static_cast<std::size_t>(i - first);
	return ilog2(diff+1);
}

template<class RAI>
RAI get_parent(RAI first, RAI last, RAI i) {
	int diff = i - first;
	if (diff < 1) {
		return last;
	}
	return first + (diff-1)/2;
}

template<class RAI>
RAI get_grand_parent(RAI first, RAI last, RAI i) {
	int diff = i - first;
	if (diff < 3) {
		return last;
	}
	return first + (diff-3)/4;
}

template<class RAI>
RAI get_smallest_child_or_grandchild(RAI first, RAI last, RAI i) {
	/* If there are no children, return last */
	RAI smallest = last;
	RAI left = get_left_child(first, last, i);
	if (left < last) {
		smallest = left;
		RAI leftLeft = get_left_child(first, last, left);
		RAI leftRight = get_right_child(first, last, left);
		if (leftLeft < last && *leftLeft < *smallest) {
			smallest = leftLeft;
		}
		if (leftRight < last && *leftRight < *smallest) {
			smallest = leftRight;
		}
	}
	RAI right = get_right_child(first, last, i);
	if (right < last) {
		if (*right < *smallest) {
			smallest = right;
		}
		RAI rightLeft = get_left_child(first, last, right);
		RAI rightRight = get_right_child(first, last, right);
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
RAI get_smallest_child_or_grandchild(RAI first,
									 RAI last,
									 RAI i,
									 Compare comp) {
	/* If there are no children, return last */
	RAI smallest = last;
	RAI left = get_left_child(first, last, i);
	if (left < last) {
		smallest = left;
		RAI leftLeft = get_left_child(first, last, left);
		RAI leftRight = get_right_child(first, last, left);
		if (leftLeft < last && comp(*leftLeft, *smallest)) {
			smallest = leftLeft;
		}
		if (leftRight < last && comp(*leftRight, *smallest)) {
			smallest = leftRight;
		}
	}
	RAI right = get_right_child(first, last, i);
	if (right < last) {
		if (comp(*right, *smallest)) {
			smallest = right;
		}
		RAI rightLeft = get_left_child(first, last, right);
		RAI rightRight = get_right_child(first, last, right);
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
RAI get_largest_child_or_grandchild(RAI first, RAI last, RAI i) {
	/* If there are no children return last */
	RAI largest = last;
	RAI left = get_left_child(first, last, i);
	if (left < last) {
		largest = left;
		RAI leftLeft = get_left_child(first, last, left);
		RAI leftRight = get_right_child(first, last, left);
		if (leftLeft < last && *leftLeft > *largest) {
			largest = leftLeft;
		}
		if (leftRight < last && *leftRight > *largest) {
			largest = leftRight;
		}
	}
	RAI right = get_right_child(first, last, i);
	if (right < last) {
		if (*right > *largest) {
			largest = right;
		}
		RAI rightLeft = get_left_child(first, last, right);
		RAI rightRight = get_right_child(first, last, right);
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
RAI get_largest_child_or_grandchild(RAI first, RAI last, RAI i, Compare comp) {
	/* If there are no children return last */
	RAI largest = last;
	RAI left = get_left_child(first, last, i);
	if (left < last) {
		largest = left;
		RAI leftLeft = get_left_child(first, last, left);
		RAI leftRight = get_right_child(first, last, left);
		if (leftLeft < last && comp(*largest, *leftLeft)) {
			largest = leftLeft;
		}
		if (leftRight < last && comp(*largest, *leftRight)) {
			largest = leftRight;
		}
	}
	RAI right = get_right_child(first, last, i);
	if (right < last) {
		if (comp(*largest, *right)) {
			largest = right;
		}
		RAI rightLeft = get_left_child(first, last, right);
		RAI rightRight = get_right_child(first, last, right);
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
void trickle_down_min(RAI first, RAI last, RAI i) {
	if (get_left_child(first, last, i) < last) {
		RAI m = get_smallest_child_or_grandchild(first, last, i);
		if (get_grand_parent(first, last, m) == i) {
			if (*m < *i) {
				std::iter_swap(m, i);
				RAI parent = get_parent(first, last, m);
				if (*m > *parent) {
					std::iter_swap(m, parent);
				}
				trickle_down_min(first, last, m);
			}
		} else {
			if (*m < *i) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI, class Compare>
void trickle_down_min(RAI first, RAI last, RAI i, Compare comp) {
	if (get_left_child(first, last, i) < last) {
		RAI m = get_smallest_child_or_grandchild(first, last, i, comp);
		if (get_grand_parent(first, last, m) == i) {
			if (comp(*m, *i)) {
				std::iter_swap(m, i);
				RAI parent = get_parent(first, last, m);
				if (comp(*parent, *m)) {
					std::iter_swap(m, parent);
				}
				trickle_down_min(first, last, m, comp);
			}
		} else {
			if (comp(*m, *i)) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI>
void trickle_down_max(RAI first, RAI last, RAI i) {
	if (get_left_child(first, last,i) < last) {
		RAI m = get_largest_child_or_grandchild(first, last, i);
		if (get_grand_parent(first, last, m) == i) {
			if (*m > *i) {
				std::iter_swap(m, i);
				RAI parent = get_parent(first, last, m);
				if (*m < *parent) {
					std::iter_swap(m, parent);
				}
				trickle_down_max(first, last, m);
			}
		} else {
			if (*m > *i) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI, class Compare>
void trickle_down_max(RAI first, RAI last, RAI i, Compare comp) {
	if (get_left_child(first, last,i) < last) {
		RAI m = get_largest_child_or_grandchild(first, last, i, comp);
		if (get_grand_parent(first, last, m) == i) {
			if (comp(*i, *m)) {
				std::iter_swap(m, i);
				RAI parent = get_parent(first, last, m);
				if (comp(*m, *parent)) {
					std::iter_swap(m, parent);
				}
				trickle_down_max(first, last, m, comp);
			}
		} else {
			if (comp(*i, *m)) {
				std::iter_swap(m, i);
			}
		}
	}
}

template<class RAI>
void trickle_down(RAI first, RAI last, RAI i) {
	if (get_level(first, last, i) % 2 == 0) {
		trickle_down_min(first, last, i);
	} else {
		trickle_down_max(first, last, i);
	}
}

template<class RAI, class Compare>
void trickle_down(RAI first, RAI last, RAI i, Compare comp) {
	if (get_level(first, last, i) % 2 == 0) {
		trickle_down_min(first, last, i, comp);
	} else {
		trickle_down_max(first, last, i, comp);
	}
}

template<class RAI>
void bubble_up_min(RAI first, RAI last, RAI i) {
	RAI gp = get_grand_parent(first, last, i);
	if (gp != last && *i < *gp) {
		std::iter_swap(i, gp);
		bubble_up_min(first, last, gp);
	}
}

template<class RAI, class Compare>
void bubble_up_min(RAI first, RAI last, RAI i, Compare comp) {
	RAI gp = get_grand_parent(first, last, i);
	if (gp != last && comp(*i, *gp)) {
		std::iter_swap(i, gp);
		bubble_up_min(first, last, gp, comp);
	}
}

template<class RAI>
void bubble_up_max(RAI first, RAI last, RAI i) {
	RAI gp = get_grand_parent(first, last, i);
	if (gp != last && *i > *gp) {
		std::iter_swap(i, gp);
		bubble_up_max(first, last, gp);
	}
}

template<class RAI, class Compare>
void bubble_up_max(RAI first, RAI last, RAI i, Compare comp) {
	RAI gp = get_grand_parent(first, last, i);
	if (gp != last && comp(*gp, *i)) {
		std::iter_swap(i, gp);
		bubble_up_max(first, last, gp, comp);
	}
}


template<class RAI>
void bubble_up(RAI first, RAI last, RAI i) {
	RAI parent = get_parent(first, last, i);
	if (get_level(first, last, i) % 2 == 0) {
		if (parent != last && *i > *parent) {
			std::iter_swap(i, parent);
			bubble_up_max(first, last, parent);
		} else {
			bubble_up_min(first, last, i);
		}
	} else {
		if (parent != last && *i < *parent) {
			std::iter_swap(i, parent);
			bubble_up_min(first, last, parent);
		} else {
			bubble_up_max(first, last, i);
		}
	}
}

template<class RAI, class Compare>
void bubble_up(RAI first, RAI last, RAI i, Compare comp) {
	RAI parent = get_parent(first, last, i);
	if (get_level(first, last, i) % 2 == 0) {
		if (parent != last && comp(*parent, *i)) {
			std::iter_swap(i, parent);
			bubble_up_max(first, last, parent, comp);
		} else {
			bubble_up_min(first, last, i, comp);
		}
	} else {
		if (parent != last && comp(*i, *parent)) {
			std::iter_swap(i, parent);
			bubble_up_min(first, last, parent, comp);
		} else {
			bubble_up_max(first, last, i, comp);
		}
	}
}

template<class RAI>
void make_minmaxheap(RAI first, RAI last) {
	if (last - first >= 2) {
		int offset = (last - first) / 2 - 1;
		for (RAI i = first + offset; i > first; --i) {
			trickle_down(first, last, i);
		}
		trickle_down(first, last, first);
	}
}

/*!
Rearranges the values in the range [first,last) as a min-max heap.
*/
template<class RAI, class Compare>
void make_minmaxheap(RAI first, RAI last, Compare comp) {
	if (last - first >= 2) {
		int offset = (last - first) / 2 - 1;
		for (RAI i = first + offset; i > first; --i) {
			trickle_down(first, last, i, comp);
		}
		trickle_down(first, last, first, comp);
	}
}

template<class RAI>
void popmin_minmaxheap(RAI first, RAI last) {
	std::iter_swap(first, last-1);
	trickle_down(first, last-1, first);
}

/*!
Moves the smallest value in the min-max heap to the end of the sequence,
shortening the actual min-max heap range by one position.
*/
template<class RAI, class Compare>
void popmin_minmaxheap(RAI first, RAI last, Compare comp) {
	std::iter_swap(first, last-1);
	trickle_down(first, last-1, first, comp);
}

template<class RAI>
void popmax_minmaxheap(RAI first, RAI last) {
	if (last-first < 2) {
		return;
	}
	if (last-first == 2 || *(first+1) > *(first+2)) {
		std::iter_swap(first+1, last-1);
		trickle_down(first, last-1, first+1);
	} else {
		std::iter_swap(first+2, last-1);
		trickle_down(first, last-1, first+2);
	}
}

/*!
Moves the largest value in the min-max heap to the end of the sequence,
shortening the actual min-max heap range by one position. */
template<class RAI, class Compare>
void popmax_minmaxheap(RAI first, RAI last, Compare comp) {
	if (last-first < 2) {
		return;
	}
	if (last-first == 2 || comp(*(first+2), *(first+1))) {
		std::iter_swap(first+1, last-1);
		trickle_down(first, last-1, first+1, comp);
	} else {
		std::iter_swap(first+2, last-1);
		trickle_down(first, last-1, first+2, comp);
	}
}

template<class RAI>
void push_minmaxheap(RAI first, RAI last) {
	bubble_up(first, last, last-1);
}

/*!
Given a min-max heap on the range [first,last), moves the element in the 
last-1) position to its correct position.
*/
template<class RAI, class Compare>
void push_minmaxheap(RAI first, RAI last, Compare comp) {
	bubble_up(first, last, last-1, comp);
}

}

#endif
