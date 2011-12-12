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

#ifndef SWAY_MIN_MAX_HEAP_HPP
#define SWAY_MIN_MAX_HEAP_HPP

#include <algorithm>
#include <sway/detail/minmaxheap.hpp>

namespace sway {

template<class RAI>
void make_minmaxheap(RAI first, RAI last) {
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	if (last - first >= 2) {
		diff_t offset = (last - first) / 2 - 1;
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
	typedef typename std::iterator_traits<RAI>::difference_type diff_t;
	if (last - first >= 2) {
		diff_t offset = (last - first) / 2 - 1;
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

/*! Given a min-max heap on the range [first,last), moves the element in the 
last-1) position to its correct position.
*/
template<class RAI, class Compare>
void push_minmaxheap(RAI first, RAI last, Compare comp) {
	bubble_up(first, last, last-1, comp);
}

}

#endif
