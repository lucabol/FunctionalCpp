#include "stdafx.h"
#include "functional.hpp"

#include <iostream>
#include <vector>
#include <functional>

#include <boost/lambda/lambda.hpp>
#include <boost/test/unit_test.hpp>
#include <pstade/oven/counting.hpp>
#include <pstade/oven/filtered.hpp>
#include <pstade/oven/transformed.hpp>
#include <pstade/oven/copied.hpp>
#include <pstade/oven/numeric.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/chrono/chrono_io.hpp>

using namespace pstade::oven;
using namespace std;
using namespace functional;
using namespace boost::lambda;
using namespace boost::chrono;

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

	return timer.elapsed();
}

int by2(int i) { return i*2;}

BOOST_AUTO_TEST_SUITE(OvenPerf)

BOOST_AUTO_TEST_CASE(OvenPerfTest)
{
#ifdef NDEBUG
	const int repeat = 100000000;
#else
	const int repeat = 1000;
#endif

	vector<int> v = counting(0l,100l) | copied;

	// fix mapped and includes 
	auto languageLambda = time_test(repeat,[&v] () -> int {

		auto lessThan50 = v | filtered([](int i) { return i < 50;})
							| transformedF([] (int i) { return i * 2;});
		return pstade::oven::accumulate (lessThan50, 0);
	});
	cout  << setw(40) << "Language lambda: " << languageLambda << endl;

	auto boostLambda = time_test(repeat,[&v] () -> int {
		auto lessThan50 = v | filtered(_1 < 50)
							| transformed(_1 * 2);

		return pstade::oven::accumulate (lessThan50, 0);
	});
	cout  << setw(40) << "Boost lambda: " << boostLambda << endl;

	auto forLambda = time_test(repeat,[&v] () -> int {
		int sum = 0;
		for(vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
			if(*it < 50)
				sum += *it * 2;
		return sum;
	});
	cout  << setw(40) << "For loop: " << forLambda << endl;

}

BOOST_AUTO_TEST_SUITE_END()