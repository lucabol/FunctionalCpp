#include "stdafx.h"

#include <vector>

#include <boost/test/unit_test.hpp>

#include "discriminated_union.hpp"

using namespace std;

DU_DECLARE(BuildAction)
    DU_FLAG (BuildAction, Reset)
    DU_VALUE(BuildAction, LinkOneWithNext,  int)
    DU_VALUE(BuildAction, LinkManyWithNext, vector<int>)
DU_END()

BOOST_AUTO_TEST_SUITE(DiscriminatedUnion)

BOOST_AUTO_TEST_CASE(DiscriminatedUnionTest)
{
	auto du = BuildAction::CreateLinkOneWithNext(3);

	DU_MATCH(du)
		DU_CASE_TAG (du, Reset,
			string s,s1 = " Error"; // testing commas in the code
			BOOST_CHECK(false);
			throw s + s1;
		)
		DU_CASE (du, LinkManyWithNext,
			BOOST_CHECK(false);
		)
		DU_CASE (du, LinkOneWithNext,
			BOOST_CHECK_EQUAL(value, 3);
		)
		DU_DEFAULT(
			throw "no match";
		)

}

BOOST_AUTO_TEST_SUITE_END()