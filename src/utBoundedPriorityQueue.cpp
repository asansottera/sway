// (C) Copyright Andrea Sansottera 2011

#include <boost/test/unit_test.hpp>

#include "BoundedPriorityQueue.h"

using namespace sway;

BOOST_AUTO_TEST_CASE(TestBPQ) {

	BoundedPriorityQueue<int> bpq(2);

	BOOST_REQUIRE_EQUAL(bpq.size(), 0u);
	
	bpq.push(10);
	BOOST_REQUIRE_EQUAL(bpq.size(), 1u);
	BOOST_CHECK_EQUAL(bpq.top(), 10);

	bpq.push(5);
	BOOST_REQUIRE_EQUAL(bpq.size(), 2u);
	BOOST_CHECK_EQUAL(bpq.top(), 5);

	bpq.push(8);
	BOOST_REQUIRE_EQUAL(bpq.size(), 2u);
	BOOST_CHECK_EQUAL(bpq.top(), 5);

	bpq.pop_top();

	BOOST_REQUIRE_EQUAL(bpq.size(), 1u);
	BOOST_CHECK_EQUAL(bpq.top(), 8);

	bpq.push(3);
	
	BOOST_REQUIRE_EQUAL(bpq.size(), 2u);
	BOOST_CHECK_EQUAL(bpq.bottom(), 8);
}

BOOST_AUTO_TEST_CASE(TestBPQ2) {

	BoundedPriorityQueue<int> bpq(3);

	BOOST_REQUIRE_EQUAL(bpq.size(), 0u);

	bpq.push(1); // 1
	bpq.push(3); // 1 3
	bpq.push(2); // 1 2 3
	bpq.push(4); // 1 2 3
	bpq.push(0); // 0 1 2

	BOOST_REQUIRE_EQUAL(bpq.size(), 3u);
	BOOST_CHECK_EQUAL(bpq.top(), 0);
	BOOST_CHECK_EQUAL(bpq.bottom(), 2);

	bpq.push(5);

	BOOST_REQUIRE_EQUAL(bpq.size(), 3u);
	BOOST_CHECK_EQUAL(bpq.top(), 0);
	BOOST_CHECK_EQUAL(bpq.bottom(), 2);
}

BOOST_AUTO_TEST_CASE(TestBPQSmall) {

	BoundedPriorityQueue<int> bpq(2);

	bpq.push(1);
	bpq.push(3);
	bpq.push(2);

	BOOST_REQUIRE_EQUAL(bpq.size(), 2u);
	BOOST_CHECK_EQUAL(bpq.top(), 1);
	BOOST_CHECK_EQUAL(bpq.bottom(), 2);
}
