// (C) Copyright Andrea Sansottera 2011

#include <boost/test/unit_test.hpp>

#include "minmaxheap.hpp"
#include <vector>
#include <iostream>
#include <stack>

using namespace std;
using namespace sway;

void CheckMinMaxHeapPropertyPtr(vector<int *> & v) {
	for (vector<int *>::iterator i = v.begin(); i < v.end(); i++) {
		bool odd = getLevel(v.begin(), v.end(), i) % 2 == 1;
		stack<vector<int *>::iterator> descendants;
		descendants.push(getLeftChild(v.begin(), v.end(), i));
		descendants.push(getRightChild(v.begin(), v.end(), i));
		while (descendants.size() > 0) {
			vector<int *>::iterator j = descendants.top();
			descendants.pop();
			if (j != v.end()) {
				if (odd)
					BOOST_REQUIRE_LE(**j, **i);
				else
					BOOST_REQUIRE_GE(**j, **i);
				descendants.push(getLeftChild(v.begin(), v.end(), j));
				descendants.push(getRightChild(v.begin(), v.end(), j));
			}
		}
	}
}

void CheckMinMaxHeapProperty(vector<int> & v) {
	for (vector<int>::iterator i = v.begin(); i < v.end(); i++) {
		bool odd = getLevel(v.begin(), v.end(), i) % 2 == 1;
		stack<vector<int>::iterator> descendants;
		descendants.push(getLeftChild(v.begin(), v.end(), i));
		descendants.push(getRightChild(v.begin(), v.end(), i));
		while (descendants.size() > 0) {
			vector<int>::iterator j = descendants.top();
			descendants.pop();
			if (j != v.end()) {
				if (odd)
					BOOST_REQUIRE_LE(*j, *i);
				else
					BOOST_REQUIRE_GE(*j, *i);
				descendants.push(getLeftChild(v.begin(), v.end(), j));
				descendants.push(getRightChild(v.begin(), v.end(), j));
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(TestMakeHeap) {

	vector<int> v;

	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	make_minmaxheap(v.begin(), v.end());

	CheckMinMaxHeapProperty(v);
}

template<class T>
struct IndirectComp {
public:
	bool operator()(T * a, T * b) {
		return *a < *b;
	}
};

BOOST_AUTO_TEST_CASE(TestMakeHeapComp) {

	int a[101];
	vector<int *> v;
	for (int i = 0; i <= 100; i++) {
		a[i] = 101 - i;
		v.push_back(a + i);
	}

	make_minmaxheap(v.begin(), v.end(), IndirectComp<int>());

	CheckMinMaxHeapPropertyPtr(v);
}

BOOST_AUTO_TEST_CASE(TestPopMinHeap) {

	vector<int> v;
	
	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	make_minmaxheap(v.begin(), v.end());

	popmin_minmaxheap(v.begin(), v.end());
	v.pop_back();

	vector<int>::iterator found = find(v.begin(), v.end(), 0);
	BOOST_REQUIRE(found == v.end());
	BOOST_REQUIRE(v.size() == 100);

	CheckMinMaxHeapProperty(v);
}

BOOST_AUTO_TEST_CASE(TestPopMinHeapComp) {

	int a[101];
	vector<int *> v;
	for (int i = 0; i <= 100; i++) {
		a[i] = 101 - i;
		v.push_back(a + i);
	}

	make_minmaxheap(v.begin(), v.end(), IndirectComp<int>());

	popmin_minmaxheap(v.begin(), v.end(), IndirectComp<int>());
	v.pop_back();

	vector<int *>::iterator found = find(v.begin(), v.end(), a + 100);
	BOOST_REQUIRE(found == v.end());
	BOOST_REQUIRE(v.size() == 100);

	CheckMinMaxHeapPropertyPtr(v);
}

BOOST_AUTO_TEST_CASE(TestPopMaxHeap) {

	vector<int> v;
	
	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	make_minmaxheap(v.begin(), v.end());

	popmax_minmaxheap(v.begin(), v.end());
	v.pop_back();

	vector<int>::iterator found = find(v.begin(), v.end(), 100);
	BOOST_REQUIRE(found == v.end());

	BOOST_REQUIRE(v.size() == 100);

	CheckMinMaxHeapProperty(v);
}

BOOST_AUTO_TEST_CASE(TestPopMaxHeapComp) {

	int a[101];
	vector<int *> v;
	for (int i = 0; i <= 100; i++) {
        a[i] = 101 - i;
        v.push_back(a + i);
    }

	make_minmaxheap(v.begin(), v.end(), IndirectComp<int>());

	popmax_minmaxheap(v.begin(), v.end(), IndirectComp<int>());
	v.pop_back();

	vector<int *>::iterator found = find(v.begin(), v.end(), a + 0);
	BOOST_REQUIRE(found == v.end());

	BOOST_REQUIRE(v.size() == 100);

	CheckMinMaxHeapPropertyPtr(v);
}

BOOST_AUTO_TEST_CASE(TestPushHeap) {

	vector<int> v;

	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	make_minmaxheap(v.begin(), v.end());

	v.push_back(200);
	push_minmaxheap(v.begin(), v.end());

	BOOST_REQUIRE(v.size() == 102);
	BOOST_CHECK(
		find(v.begin(), v.end(), 200) == v.begin() + 1 ||
		find(v.begin(), v.end(), 200) == v.begin() + 2);

	CheckMinMaxHeapProperty(v);

}

BOOST_AUTO_TEST_CASE(TestPushHeapComp) {

	int a[101];
	vector<int *> v;
	for (int i = 0; i <= 100; i++) {
        a[i] = 101 - i;
        v.push_back(a + i);
    }
	int b = 200;

	make_minmaxheap(v.begin(), v.end(), IndirectComp<int>());

	v.push_back(&b);
	push_minmaxheap(v.begin(), v.end(), IndirectComp<int>());

	BOOST_REQUIRE(v.size() == 102);
	BOOST_CHECK(
		find(v.begin(), v.end(), &b) == v.begin() + 1 ||
		find(v.begin(), v.end(), &b) == v.begin() + 2);

	CheckMinMaxHeapPropertyPtr(v);

}

BOOST_AUTO_TEST_CASE(TestMakeSmall2) {

	vector<int> a;
	a.push_back(10);
	a.push_back(5);

	make_minmaxheap(a.begin(), a.end());

	BOOST_CHECK_EQUAL(a[0], 5);
	BOOST_CHECK_EQUAL(a[1], 10);
}

BOOST_AUTO_TEST_CASE(TestPushSmall12) {

	vector<int> a;
	a.push_back(10);

	make_minmaxheap(a.begin(), a.end());

	a.push_back(5);
	push_minmaxheap(a.begin(), a.end());

	BOOST_CHECK_EQUAL(a[0], 5);
	BOOST_CHECK_EQUAL(a[1], 10);
}

BOOST_AUTO_TEST_CASE(TestPushSmall02) {

	vector<int> a;

	make_minmaxheap(a.begin(), a.end());
	
	a.push_back(10);
	push_minmaxheap(a.begin(), a.end());
	
	a.push_back(5);
	push_minmaxheap(a.begin(), a.end());

	BOOST_CHECK_EQUAL(a[0], 5);
	BOOST_CHECK_EQUAL(a[1], 10);
}

BOOST_AUTO_TEST_CASE(TestSmallExceed) {

	vector<int> a;
	a.push_back(10);
	a.push_back(5);

	make_minmaxheap(a.begin(), a.end());

	BOOST_CHECK_EQUAL(a[0], 5);
	BOOST_CHECK_EQUAL(a[1], 10);

	popmax_minmaxheap(a.begin(), a.end());
	a.pop_back();

	BOOST_CHECK_EQUAL(a[0], 5);
}
