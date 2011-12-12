/*
Copyright (c) 2011, Andrea Sansottera
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
Based on the research paper:
	Min-Max Heaps and Generalized Priority Queues
	M. D. Atkinson, J. R. Sack, N. Santoro and T. Strothotte
	Communications of the ACM, October 1986
*/


#ifndef SWAY_DETAIL_MIN_MAX_HEAP_HPP
#define SWAY_DETAIL_MIN_MAX_HEAP_HPP

#include <algorithm>
#include <sway/ilog2.hpp>

namespace sway {

template<class RAI>
RAI get_left_child(RAI first, RAI last, RAI i) {
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	diff_t offset = (i - first) * 2 + 1;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
RAI get_right_child(RAI first, RAI last, RAI i) {
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	diff_t offset = (i - first) * 2 + 2;
	if (offset >= last - first) {
		return last;
	} else {
		return first + offset;
	}
}

template<class RAI>
std::size_t get_level(RAI first, RAI last, RAI i) {
	// the difference cannot be negative, so we can cast to unsigned type
	std::size_t diff = static_cast<std::size_t>(i - first);
	return ilog2(diff+1);
}

template<class RAI>
RAI get_parent(RAI first, RAI last, RAI i) {
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	diff_t diff = i - first;
	if (diff < 1) {
		return last;
	}
	return first + (diff-1)/2;
}

template<class RAI>
RAI get_grand_parent(RAI first, RAI last, RAI i) {
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	diff_t diff = i - first;
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

}

#endif
