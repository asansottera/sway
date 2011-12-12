// (C) Copyright Andrea Sansottera 2011

#ifndef BOUNDED_PRIORITY_QUEUE_H
#define BOUNDED_PRIORITY_QUEUE_H

#include "MinMaxHeap.h"
#include <functional>
#include <vector>

namespace sway {

/*!
This template class is a container adapter, implementing a priority queue with 
a bounded the number of items.
Once the queue is full, the elements with the smallest priority are dropped.
The implementation is based on the min-max heap implicit data structure.
If no container template parameter is specified, a vector is used.
If no comparer template parameter is specified, the < operator is used.
*/
template<class T,
		 class Container = std::vector<T>,
		 class Compare = std::less<T> >
class BoundedPriorityQueue {
private:
	std::size_t m_count;
	std::vector<T> m_heap;
	Compare m_comp;
public:
	/*!
	Constructs an empty bounded priority queue of the given size.
	*/
	BoundedPriorityQueue(std::size_t size, const Compare & comp = Compare())
		: m_count(0), m_heap(size), m_comp(comp) {
	}
	/*!
	Constructs a bounded priority queue containing the items of the
	container. The maximum size of the priority queue is equal to the size
	of the container.
	*/
	BoundedPriorityQueue(const Container & container,
						 const Compare & comp = Compare())
		: m_count(0), m_heap(container.size()), m_comp(comp) {
		typename Container::const_iterator itr;
		for (itr = container.begin(); itr != container.end(); ++itr) {
			push(*itr);
		}
	}
	/*!
	Constructs a bounded priority queue containing some of the items of the
	container. The maximum size of the priority queue is provided as a
	paramater and, in general, might be smaller than the size of the container.
	In this case, the lowest priority elements are not inserted.
	*/
	BoundedPriorityQueue(std::size_t size,
						 const Container & container,
						 const Compare & comp = Compare())
		: m_count(0), m_heap(size), m_comp(comp) {
		typename Container::const_iterator itr;
		for (itr = container.begin(); itr != container.end(); ++itr) {
			push(*itr);
		}
	}
	/*!
	Tries to add a new element to the queue.
	If the queue is full and the new element has a equal or higher priority
	than the bottom element, the queue is left unmodified.
	If the queue is full and the new element has a lower priority
	than the bottom element, the bottom element is removed and the new element
	is inserted.
	*/
	void push(const T & obj) {
		if (m_count == m_heap.size()) {
			if (m_comp(obj, bottom())) {
				popmax_minmaxheap(m_heap.begin(), m_heap.end(), m_comp);
				*m_heap.rbegin() = obj;
				push_minmaxheap(m_heap.begin(),
								m_heap.begin() + m_count,
								m_comp);
			}
		} else {
			m_heap[m_count] = obj;
			++m_count;
			push_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
		}

	}
	/*!
	Returns a reference to the highest priority element of the queue.
	*/
	const T & top() const {
		return m_heap[0];
	}
	/*!
	Returns a reference to the lowest priority element of the queue.
	*/
	const T & bottom() const {
		if (m_count == 1) {
			return m_heap[0];
		}
		if (m_count == 2) {
			return m_heap[1];
		}
		return m_comp(m_heap[2], m_heap[1]) ? m_heap[1] : m_heap[2];
	}
	/*!
	Removes the highest priority element of the queue.
	*/
	void pop_top() {
		popmin_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
		--m_count;
	}
	/*!
	Removes the lowest priority element of the queue.
	*/
	void pop_bottom() {
		popmax_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
		--m_count;
	}
	/*!
	Returns the number of elements stored in the queue.
	*/
	std::size_t size() const {
		return m_count;
	}
	/*!
	Returns true if the queue has no elements, false otherwise.
	*/
	bool empty() const {
		return m_count == 0;
	}
};

}

#endif
