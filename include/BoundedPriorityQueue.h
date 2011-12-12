// (C) Copyright Andrea Sansottera 2011

#ifndef BOUNDED_PRIORITY_QUEUE_H
#define BOUNDED_PRIORITY_QUEUE_H

#include "MinMaxHeap.h"
#include <functional>
#include <vector>

namespace sway {

/*!
This data structures is a priority queue with a bound on the number of items.
Once the queue is full, the elements with the smallest priority are dropped.
The implementation is based on the min-max heap implicit data structure (over an array).
If no comparer functor is specified the default is to use the operator <.
*/
template<class T, class Container = std::vector<T>, class Compare = std::less<T> >
class BoundedPriorityQueue {
private:
	std::size_t m_size;
	std::size_t m_count;
	std::vector<T> m_heap;
	Compare m_comp;
public:
	BoundedPriorityQueue(std::size_t size, Compare comp = Compare()) : m_heap(size) {
		m_size = size;
		m_count = 0;
		m_comp = comp;
	}
	/*!
	Tries to add a new element to the queue.
	If the queue is full either this element or one previously stored is dropped.
	*/
	void push(T obj) {
		if (m_count == m_size) {
			if (m_comp(obj, bottom())) {
				popmax_minmaxheap(m_heap.begin(), m_heap.end(), m_comp);
				m_heap[m_size - 1] = obj;
				push_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
			}
		} else {
			m_heap[m_count] = obj;
			m_count++;
			push_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
		}

	}
	/*!
	Returns a reference to the highest priority element of the queue (by default, the smallest).
	*/
	const T & top() const {
		return m_heap[0];
	}
	/*!
	Returns a reference to the lowest priority element of the queue (by default, the largest).
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
	Removes the highest priority element of the queue (by default, the smallest).
	*/
	void pop() {
		popmin_minmaxheap(m_heap.begin(), m_heap.begin() + m_count, m_comp);
		m_count--;
	}
	/*!
	Returns the number of elements stored in the queue.
	*/
	std::size_t size() const {
		return m_count;
	}
};

}

#endif
