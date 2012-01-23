#include "stdafx.h"
#include "ufunctional.hpp"

using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::range;

using namespace ufunctional;

struct int_to_string {
	typedef string result_type;

	auto operator()(int number) const -> string {
	   stringstream ss;
	   ss << number;
	   return ss.str();
	}
};

template<class T>
auto alwaysTrue(T) RETURNS ( true );

bool alwaysTrue1 (int) { return true;}

BOOST_AUTO_TEST_SUITE(Ranges)

BOOST_AUTO_TEST_CASE(RangeTest)
{
	const int tmp[] = {1,2,3,4,5,6,7,8,9,0};
	const char* tmpS[] = {"1","2","3","4","5","6","7","8","9","0"};
	const vector<int> v(&tmp[0], &tmp[10]);
	const vector<int> originalV(v);
	const int evenInts[] = {2,4,6,8,0};

	// ufilter doesn't modify the original range and works
	auto filteredRange = ufilter(v, [](int x) { return x % 2 == 0;});
	BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), originalV.begin(), originalV.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(filteredRange), boost::end(filteredRange), &evenInts[0], &evenInts[5]);

	// ufilter is composeable
	auto filteredRange2 = ufilter(filteredRange, alwaysTrue1);
	auto filteredRange3 = ufilter(filteredRange2, alwaysTrue<int>);
	BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), originalV.begin(), originalV.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(filteredRange3), boost::end(filteredRange3), &evenInts[0], &evenInts[5]);

	auto replacedRange = filteredRange | replaced(0,10) | reversed;
	cout << "After operation: " << v << endl;
	cout << "Replaced/reversed: " << replacedRange << endl;

	auto transformedRange = replacedRange | transformed(int_to_string());
	cout << "After operation: " << v << endl;
	cout << "Transformed: " << transformedRange << endl;


	auto sortedRange = usort(v, [](int x, int y) { return x > y;});
	cout << "After operation: " << v << endl;
	cout << "Sorted: " << sortedRange << endl;

	auto vs = umap<vector<string>>(v, [](int x) {return int_to_string()(x);});
	BOOST_CHECK_EQUAL_COLLECTIONS(vs.begin(), vs.end(), &tmpS[0], &tmpS[10]);

	cout << "After operation: " << v << endl;
	cout << "Strings: " << vs << endl;

	int acc = ufold(v, 0, [](int x, int y) {return x + y;});
	cout << "After operation: " << v << endl;
	cout << "Sum: " << acc << endl;
}

BOOST_AUTO_TEST_SUITE_END()
