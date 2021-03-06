#include "stdafx.h"

#include <vector>

#include <boost/test/unit_test.hpp>

#include "discriminated_union.hpp"

using namespace std;

DU_DECLARE(BuildAction)
    DU_FLAG (Reset)
    DU_VALUE(LinkOneWithNext,  int)
    DU_VALUE(LinkManyWithNext, vector<int>)
DU_END

DU_DECLARE(Switch)
	DU_FLAG(On)
	DU_FLAG(Off)
DU_END

DU_DECLARE(LivingEntity)
    DU_VALUE(Person,	string)
    DU_VALUE(Dog,		string)
DU_END

auto print(LivingEntity en) -> void {
	DU_MATCH(en)
		DU_CASE(Dog,	cout << "I'm a per named " << value;)
		DU_CASE(Person, cout << "I'm a dog named " << value;) 
	DU_MATCH_END
}

BOOST_AUTO_TEST_SUITE(DiscriminatedUnion)

BOOST_AUTO_TEST_CASE(DiscriminatedUnionTest)
{
	auto du = BuildAction::LinkOneWithNext(3);

	DU_MATCH(du)
		DU_CASE_TAG  (Reset,
			string s,s1 = " Error"; // testing commas in the code, should be catch by __VAR_ARGS__
			BOOST_CHECK(false);
			throw s + s1;
		)
		DU_CASE		(LinkManyWithNext, BOOST_CHECK(false);)
		DU_CASE		(LinkOneWithNext,  BOOST_CHECK_EQUAL(value, 3);)
		DU_DEFAULT	(				   throw "no match";)
	DU_MATCH_END
}

BOOST_AUTO_TEST_CASE(DiscriminatedUnionTest2)
{
	auto entity = LivingEntity::Dog("Bob");

	DU_MATCH(entity)
		DU_CASE(Dog, BOOST_CHECK_EQUAL(value, "Bob");)
		DU_CASE(Person,BOOST_CHECK(false);) 
	DU_MATCH_END

	DU_MATCH(entity)
		DU_CASE(Dog,
			//cout << "I should be here";
			BOOST_CHECK_EQUAL(value, "Bob");
		)
		DU_CASE(Person,
			BOOST_CHECK(false);
		) 
	DU_MATCH_END

	DU_MATCH(entity)
		DU_CASE(Dog,
		{
			//cout << "I should be here";
			BOOST_CHECK_EQUAL(value, "Bob");
		})
		DU_CASE(Person,
		{
			BOOST_CHECK(false);
		}) 
	DU_MATCH_END

	//entity = LivingEntity::Dog("Rob");

	//if(false) {}
	//	else if(entity.IsDog()) {
	//		auto value = entity.GetDog();
	//		BOOST_CHECK_EQUAL(value, "Bob");
	//	}
	//	else if(entity.IsPerson()) {
	//		auto value = entity.GetPerson();
	//		BOOST_CHECK(false);
	//	} 
	//	else {
	//		throw new match_exception();
	//	}
}

BOOST_AUTO_TEST_SUITE_END()

//struct LivingEntity {
//	private:
//		LivingEntity() {}
//		unsigned int m_kind;
//	public:
//		static LivingEntity Person(const string& value) {
//			LivingEntity unionValue;
//			unionValue.m_kind = 19;
//			unionValue.m_Person = value;
//			return unionValue; }
//
//		bool IsPerson() const {
//			return m_kind == 19;
//		}
//		const string& GetPerson() const {
//			(void)( (!!(m_kind == 19)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 19), 0) );
//			return m_Person; }
//		string GetPerson() {
//			(void)( (!!(m_kind == 19)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 19), 0) );
//			return m_Person; }
//		private: string m_Person;
//	
//public:
//		static LivingEntity Dog(const string& value) { LivingEntity unionValue; unionValue.m_kind = 20; unionValue.m_Dog = value; return unionValue; } bool IsDog() const { return m_kind == 20;} const string& GetDog() const { (void)( (!!(m_kind == 20)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 20), 0) ); return m_Dog; } string GetDog() { (void)( (!!(m_kind == 20)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 20), 0) ); return m_Dog; } private: string m_Dog; public:
//		static LivingEntity Doggy(const string& value) { LivingEntity unionValue; unionValue.m_kind = 21; unionValue.m_Doggy = value; return unionValue; } bool IsDoggy() const { return m_kind == 21;} const string& GetDoggy() const { (void)( (!!(m_kind == 21)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 21), 0) ); return m_Doggy; } string GetDoggy() { (void)( (!!(m_kind == 21)) || (_wassert(L"m_kind == __LINE__", L"c:\\projects\\functionalcpp\\discriminated_union.cpp", 21), 0) ); return m_Doggy; } private: string m_Doggy; public:
//};
