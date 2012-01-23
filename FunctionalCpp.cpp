// FunctionalCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::range;

#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")

template<class T>
ostream& operator<<(ostream& s, vector<T> const& vec) {
	copy(vec.begin(), vec.end(), ostream_iterator<T>(s, " "));
	return s;
}

struct int_to_string {
	typedef string result_type;

	auto operator()(int number) const -> string {
	   stringstream ss;
	   ss << number * 2;
	   return ss.str();
	}
};

template<class Range>
auto l_sort(Range r) -> Range {return sort(r1);};

template<class Range, class F>
auto l_sort(Range r, F f) -> Range { return sort(r, f);};

template<class Container, class Range, class F>
auto l_map(const Range& r, F f) -> Container {
	Container c;
	transform(r, back_inserter(c), f);
	return c;
}

template<class Range, class T, class F>
auto l_fold(const Range& r, T init, F f) -> T { return accumulate(r, init, f);}

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