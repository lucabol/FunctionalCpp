#include "stdafx.h"

#include "functional.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <pstade/oven/numeric.hpp>
#include <pstade/oven/counting.hpp>
#include <pstade/oven/filtered.hpp>
#include <pstade/oven/transformed.hpp>
#include <pstade/oven/equals.hpp>
#include <pstade/oven/sorted.hpp>

using namespace std;

using namespace functional;
using namespace pstade::oven;

template<class T>
auto alwaysTrue(T) RETURNS ( true );

bool alwaysTrue1 (int) { return true;}

#define NON_DESTRUCTIVE BOOST_CHECK(equals(v, originalV))

BOOST_AUTO_TEST_SUITE(Ranges)

BOOST_AUTO_TEST_CASE(RangeTest)
{
	const int tmp[] = {1,2,3,4,5,6,7,8,9,0};
	const int sortedArr[] = {0,1,2,3,4,5,6,7,8,9};
	const char* tmpS[] = {"1","2","3","4","5","6","7","8","9","0"};
	vector<int> v(&tmp[0], &tmp[10]);
	const vector<int> originalV(v);
	const int evenInts[] = {2,4,6,8,0};
	const int replacedInts[] = {2,4,6,8,10};

	// filter doesn't modify the original range and works
	auto filteredRange = v | filtered([](int x) { return x % 2 == 0;});
	NON_DESTRUCTIVE;
	BOOST_CHECK(equals(filteredRange, evenInts));

	// operators are composeable
	auto filteredRange2 = filteredRange
						  | filtered(alwaysTrue1)
						  | filtered(alwaysTrue<int>);
	NON_DESTRUCTIVE;
	BOOST_CHECK(equals(filteredRange2, evenInts));

	// you need a different function to transform using lambdas and normal functions
	auto replacedRange = filteredRange | transformedF([](int x) -> int { if(x == 0) return 10; else return x;});
	NON_DESTRUCTIVE;
	BOOST_CHECK(equals(replacedRange, replacedInts));

	// it works well for functors
	auto transformedRange = v | transformed(boost::lexical_cast<string, int>);
	NON_DESTRUCTIVE;
	BOOST_CHECK(equals(transformedRange, tmpS));

	// even sorting is not destructive
	auto sortedRange = v | sorted([](int x, int y) { return x < y;});
	NON_DESTRUCTIVE;
	BOOST_CHECK(equals(sortedRange, sortedArr));

	int acc = pstade::oven::accumulate(v, 0, [](int x, int y) {return x + y;});
	NON_DESTRUCTIVE;
	BOOST_CHECK_EQUAL(acc, 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 0);
}

BOOST_AUTO_TEST_SUITE_END()
