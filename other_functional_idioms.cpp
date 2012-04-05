#include "stdafx.h"

#include <boost/test/unit_test.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

//using namespace boost::tuples;
using std::string;
using namespace std;;

BOOST_AUTO_TEST_SUITE(DiscriminatedUnion)

BOOST_AUTO_TEST_CASE(NestedFunctions)
{
	int x = 3;
	auto sumX = [&] (int y) { return x + y;};

	BOOST_CHECK_EQUAL(sumX(2), 3+2);
}

BOOST_AUTO_TEST_CASE(Tuples)
{
	auto t = make_tuple("bob", "john", 3, 2.3);
	BOOST_CHECK_EQUAL(get<0>(t), "bob");
	BOOST_CHECK_EQUAL(get<2>(t), 3);

	// yep, compiler error
	//auto k = get<10>(t);

	auto t2(t);
	BOOST_CHECK(t2 == t);

	// passing as argument, returning it
	auto f = [] (tuple<string, string, int, double> t) { return t;};
	auto t1 = f(t);
	BOOST_CHECK(t == t1);

	// automatic deconstruction
	string s1; string s2; int i; double d;
	std::tie(s1, s2, i, d) = f(t);
	BOOST_CHECK_EQUAL(s1, "bob");

	// partial reconstruction
	string s11;
	std::tie(s11, ignore, ignore, ignore) = f(t);
	BOOST_CHECK_EQUAL(s11, "bob");
}

BOOST_AUTO_TEST_SUITE_END()
