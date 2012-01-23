// FunctionalCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "l_functional.hpp"

using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::range;


BOOST_AUTO_TEST_SUITE(Ranges)

BOOST_AUTO_TEST_CASE(RangeTest)
{
	int tmp[] = {1,2,3,4,5,6,7,8,9,0};
	vector<int> v(&tmp[0], &tmp[10]);
	cout << "Starts as: " << v << endl;

	auto filteredRange = v | filtered([](int x) { return x % 2 == 0;});
	cout << "After operation: " << v << endl;
	cout << "Filtered: " << filteredRange << endl;

	auto replacedRange = filteredRange | replaced(0,10) | reversed;
	cout << "After operation: " << v << endl;
	cout << "Replaced/reversed: " << replacedRange << endl;

	auto transformedRange = replacedRange | transformed(int_to_string());
	cout << "After operation: " << v << endl;
	cout << "Transformed: " << transformedRange << endl;


	auto sortedRange = l_sort(v, [](int x, int y) { return x > y;});
	cout << "After operation: " << v << endl;
	cout << "Sorted: " << sortedRange << endl;

	vector<string> vs = l_map<vector<string>>(v, [](int x) {return int_to_string()(x);});
	cout << "After operation: " << v << endl;
	cout << "Strings: " << vs << endl;

	int acc = l_fold(v, 0, [](int x, int y) {return x + y;});
	cout << "After operation: " << v << endl;
	cout << "Sum: " << acc << endl;
}

BOOST_AUTO_TEST_SUITE_END()