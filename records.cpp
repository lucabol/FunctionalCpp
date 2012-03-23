#include "stdafx.h"

#include <vector>

#include <boost/test/unit_test.hpp>

#include "functional.hpp"

#ifdef __GNUC__

#else
	#pragma warning (disable: 4351)
	#pragma warning (disable: 4996)
#endif

using namespace std;

namespace detail {
	struct _Person {
		int id;
		char name[20];

		pod_equality(_Person);
	};	
}

typedef const detail::_Person CPerson;
typedef detail::_Person MPerson;



MPerson ChangePerson1(const MPerson& p) {
	MPerson temp(p);
	temp.id = 4;
	return temp;
}

MPerson ChangePerson2(MPerson p) {
	p.id = 4;
	return p;
}

struct Person {
	const int Id;
	const string Name;

	Person(int id, string name): Id(id), Name(name) {}

    const Person& operator=(const Person& other) {
		static_cast<string>(this->Name) = other.Name;
		static_cast<int>(this->Id)		= other.Id;
		return *this;
	}

    bool operator==(const Person& other) const { return Id == other.Id && Name == other.Name;}
    bool operator!=(const Person& other) const { return !(*this == other);}
};

template<class Record>
class pod_equal {
    bool operator==(const Record& other) const {                                                 \
            static_assert(std::is_trivial<Record>::value, "Not trivially copyable");       \
            return memcmp(this, &other, sizeof(Record)) == 0;}                             \
    bool operator!=(const Record& other) const { return !(*this == other);}
};

struct Person2/*: pod_equal<Person2>*/ {
	int Id;
	char Name[20];

	pod_equality(Person2);
};

// const members are very messy to initialize
struct ConstRecord {
	const int Id;
	const char Name[20];

	// Wow, look at const_cast, really it should be nonconst with standard accessors ...
	ConstRecord(): Id(0), Name() { strcpy(const_cast<char*>(Name), "bobby");};

    bool operator==(const ConstRecord& other) const { return Id == other.Id && !strcmp(Name, other.Name);}
    bool operator!=(const ConstRecord& other) const { return !(*this == other);}
};

// This works
struct Record {
	int Id;
	char Name[20];

	pod_equality(Record);
};

// string members break it
struct RecordS {
	int Id;
	string Name;

    bool operator==(const RecordS& other) const { return Id == other.Id && Name == other.Name;}
    bool operator!=(const RecordS& other) const { return !(*this == other);}
};

template<class T>
bool bit_copyable(T t) {
	const size_t size = sizeof(T);
	unique_ptr<unsigned char[]> buf(new unsigned char[size]);
	memcpy(buf.get(), &t, size);
	T* cp = (T*) buf.get();
	return *cp == t;
}


BOOST_AUTO_TEST_SUITE(RecordsSuite)

BOOST_AUTO_TEST_CASE(RecordsTest)
{
	// standard layout means that it is layed out the same in C and C++
	// trivial means that it support static initialization syntax (even if RecordS does and it's not trivial)
	// http://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821#7189821

	ConstRecord cr;
	// Having a const member makes it not C usable
	BOOST_CHECK(! std::is_trivial<ConstRecord>::value);
	#ifdef _MSC_VER
	BOOST_CHECK(! std::is_standard_layout<ConstRecord>::value);
	#else
    BOOST_CHECK( std::is_standard_layout<ConstRecord>::value);

	#endif

	BOOST_CHECK(! std::is_pod<ConstRecord>::value);
	BOOST_CHECK(bit_copyable(cr));

	// Nice construction syntax
	RecordS rs = {2, "bobby"};
	BOOST_CHECK(! std::is_trivial<RecordS>::value);
	#ifdef _MSC_VER
	BOOST_CHECK(! std::is_standard_layout<RecordS>::value);
	#else
	BOOST_CHECK( std::is_standard_layout<RecordS>::value);
	#endif
	BOOST_CHECK(! std::is_pod<RecordS>::value);
	BOOST_CHECK(bit_copyable(rs)); // but it is trivially copyable??

	// Nice construction syntax
	Record r = {2, "bobby"};
	BOOST_CHECK(std::is_trivial<Record>::value);
	BOOST_CHECK(std::is_standard_layout<Record>::value);
	BOOST_CHECK(std::is_pod<Record>::value);
	BOOST_CHECK(bit_copyable(r));

	// copy works
	auto rcopy = r;
	BOOST_CHECK(r == rcopy);

	// initializatin works
	auto rcopy2(r);
	BOOST_CHECK(! (r != rcopy2));

	// passing ref and value works
	auto f = [&] (Record rvalue, Record rref) { return rvalue == rref && rvalue == r;};
	BOOST_CHECK(f(r, rcopy));

	Person2 p2 = {3, "Rob"};
	Person2 p3 = {3, "Rob"};
	BOOST_CHECK(p2 == p3);

	CPerson cp1 = {2, "Bob"};
	CPerson cp2 = {2, "Bob"};
	BOOST_CHECK(cp1 == cp2);

	// Cannot assign to a const struct
	//cp1.id = 3;
}

BOOST_AUTO_TEST_SUITE_END()
