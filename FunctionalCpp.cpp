#include "stdafx.h"
#include "ufunctional.hpp"

using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::range;

using namespace ufunctional;

template<class T>
auto alwaysTrue(T) RETURNS ( true );

bool alwaysTrue1 (int) { return true;}

#define NON_DESTRUCTIVE CHECK_ITEMS(v, originalV)

BOOST_AUTO_TEST_SUITE(Ranges)

BOOST_AUTO_TEST_CASE(RangeTest)
{
	const int tmp[] = {1,2,3,4,5,6,7,8,9,0};
	const char* tmpS[] = {"1","2","3","4","5","6","7","8","9","0"};
	vector<int> v(&tmp[0], &tmp[10]);
	const vector<int> originalV(v);
	const int evenInts[] = {2,4,6,8,0};
	const int replacedInts[] = {2,4,6,8,10};

	// ufilter doesn't modify the original range and works
	auto filteredRange = v | filtered([](int x) { return x % 2 == 0;});
	NON_DESTRUCTIVE;
	CHECK_ITEMS(filteredRange, evenInts);

	// ufilter is composeable
	auto filteredRange2 = filteredRange | filtered(alwaysTrue1);
	auto filteredRange3 = filteredRange2 | filtered(alwaysTrue<int>);
	NON_DESTRUCTIVE;
	CHECK_ITEMS(filteredRange3, evenInts);

	auto replacedRange = filteredRange | replaced(0,10);
	NON_DESTRUCTIVE;
	CHECK_ITEMS(replacedRange, replacedInts);

	auto replacedRange2 = replacedRange | replaced_if([](int x) {return x == 10;}, 0);
	NON_DESTRUCTIVE;
	CHECK_ITEMS(replacedRange2, evenInts);

	auto transformedRange = v | transformed(to_string<int>);
	NON_DESTRUCTIVE;
	CHECK_ITEMS(transformedRange, tmpS);

	// using the non-destructive home made version ...
	auto sortedRange = usort(v, [](int x, int y) { return x > y;});
	NON_DESTRUCTIVE;

	// MSVC function<> doesn't have a result_type type? Check gcc ...
	function<string(int)> f = [](int x) {return to_string(x);};
	auto vs = v | transformed(f);
	NON_DESTRUCTIVE;
	CHECK_ITEMS(vs, tmpS);

	int acc = accumulate(v, 0, [](int x, int y) {return x + y;});
	NON_DESTRUCTIVE;
	BOOST_CHECK_EQUAL(acc, 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 0);
}

BOOST_AUTO_TEST_SUITE_END()
