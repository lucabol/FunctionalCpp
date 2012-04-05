#include "stdafx.h"
#include "functional.hpp"

#include <iostream>
#include <vector>
#include <functional>

#include <boost/lambda/lambda.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/taken.hpp>
#include <boost/range/to_container.hpp>
#include <boost/range/numeric.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/chrono/chrono_io.hpp>
#include <boost/range/regular.hpp>
#include <boost/range/iteration.hpp>

using namespace std;
using namespace functional;
using namespace boost::lambda;
using namespace boost::chrono;
using namespace boost::range;
using namespace boost::adaptors;


namespace {

typedef boost::chrono::process_cpu_clock the_clock;

struct timer {
	timer(): clock_(the_clock::now()) {}

	the_clock::times elapsed() {
		return (the_clock::now() - clock_).count();
	}

	the_clock::time_point clock_;
};

the_clock::times time_test(int repeat, function<int (void)> f) {

	timer timer;

	long sum = 0;
	for(int i = 0; i < repeat; ++i)
		sum += f();

	auto ret = timer.elapsed();
	return ret;
}

int by2(int i) { return i*2;}

BOOST_AUTO_TEST_SUITE(OvenPerf)

struct Doubler {
	typedef int result_type;
	int operator() (int i) const { return i * 2;}
};

struct Filterer {
	typedef int result_type;
	bool operator() (int i) const { return i < 50;}
};


BOOST_AUTO_TEST_CASE(OvenPerfTest)
{
#ifdef NDEBUG
	const int repeat = 10000000;
#else
	const int repeat = 1000;
#endif

    function<int(int)> next = [](int i) {return i+1;};
	vector<int> v = boost::iteration(1, next) | taken(100) | boost::to_container;

	auto languageLambda = time_test(repeat,[&v] () -> int {

		auto lessThan50 = v | filtered(    [](int i) { return i < 50;})
							| transformedF([] (int i) { return i * 2;});

		return boost::accumulate (lessThan50, 0);
	});
	cout  << setw(40) << "Language lambda: " << languageLambda << endl;

	auto functorLambda = time_test(repeat,[&v] () -> int {

		auto lessThan50 = v | filtered(Filterer())
							| transformed(Doubler());

		return boost::accumulate (lessThan50, 0);
	});
	cout  << setw(40) << "Functor lambda: " << functorLambda << endl;

	auto boostLambda = time_test(repeat,[&v] () -> int {
		auto lessThan50 = v | filtered(_1 < 50)
							| transformed(_1 * 2);

		return boost::accumulate (lessThan50, 0);
	});
	cout  << setw(40) << "Boost lambda: " << boostLambda << endl;

	auto forLambda = time_test(repeat,[&v] () -> int {
		int sum = 0;
		for(vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
			if(*it < 50 == 0)
				sum += *it * 2;
		return sum;
	});
	cout  << setw(40) << "For loop: " << forLambda << endl;

	auto stlForEach = time_test(repeat,[&v] () -> int {
        int sum = 0;
        std::for_each( v.cbegin(), v.cend(),
            [&](int i){
                if( i < 50 ) sum += i*2; 
            });
        return sum;
	});
	cout  << setw(40) << "STL foreach: " << forLambda << endl;

#ifdef NDEBUG
	BOOST_CHECK(functorLambda < (long) forLambda * 2);
	BOOST_CHECK(languageLambda < (long)forLambda * 2);
	BOOST_CHECK(boostLambda < (long)forLambda * 2);
#endif

}

BOOST_AUTO_TEST_SUITE_END()

}
