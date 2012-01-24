#include "stdafx.h"
#include "ufunctional.hpp"
#include "ulazylist.hpp"

using namespace ufunctional;

BOOST_AUTO_TEST_SUITE(LazyListSuite)

BOOST_AUTO_TEST_CASE(LazyList)
{
    auto count = ufrom<int>(0, [](int i) {return i+1;});
    auto ints = {0,1,2,3,4};
    auto k = utake(5,count);
    CHECK_ITEMS(k, ints);

    auto pluses = ufrom<string>("+", [] (string s) { return s + "+";});
    auto fivePluses = utake(5,pluses);
    vector<string> fives = {"+", "++", "+++", "++++", "+++++"};
    CHECK_ITEMS(fivePluses, fives);
}

BOOST_AUTO_TEST_SUITE_END()
