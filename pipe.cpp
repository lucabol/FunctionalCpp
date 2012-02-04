#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include "functional.hpp"
#include <boost/iterator_adaptors.hpp>
#include <boost/range/iteration.hpp>
#include <boost/range/adaptor/taken.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptor/filtered.hpp>

using namespace std;
using namespace functional;
using namespace boost::adaptors;

namespace {

auto by2 (int i) RETURNS(i * 2);
int by2f(int i) { return i * 2;}

template<class Range, class Pred>
auto filterRange(Range r, Pred pred) RETURNS( r | filtered(pred) );

BOOST_AUTO_TEST_SUITE(PipeSuite)

BOOST_AUTO_TEST_CASE(PipeTest)
{
	// Function defined as lambda
	BOOST_CHECK_EQUAL(pipe(2, by2, by2), 8);
	// Function defined as normal
	BOOST_CHECK_EQUAL(pipe(2, by2f, by2), 8);

	// Calling a standard function and different codomain
	BOOST_CHECK_EQUAL(pipe("bobby", strlen), 5u);

	// Shows usage of counting
	auto toTest = counting(0, 10);
	auto result = counting(0, 5);

	BOOST_CHECK(boost::equal(filterRange(toTest, [](int x) {return x < 5;}), result));

	// Can call templates
	BOOST_CHECK_EQUAL(pipe("bobby", strlen, boost::lexical_cast<string, int>), "5");
}

BOOST_AUTO_TEST_SUITE_END()

}
