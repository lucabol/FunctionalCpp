// dbc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "dbc.hpp"

#include <boost/test/unit_test.hpp>

using namespace std;

// requires is for preconditions, ensures is for postconditions
// you can have multiple requires and ensures or simply use && in the clause
// if you want to refer to the result of the funtion in the postcondition, you need to declare it before the ensures
double divide(double x, double y) {
	double result;
	requires(y != 0);
	requires(y != 0 && y != 0)

	ensures(result < x);
	ensures(result < x + 1);

	result = y / x;
	return result;
}

class Counter {
public:
	explicit Counter():c_(0) {}

	// in a class, you can have an invariant (isValid) function that gets translated to pre and post condition
	// you can also use an ensuresClass if you want to capture the initial state of your object, compared to the state
	// at the end of the method
	void Add(int x) {
		invariant();
		requires(x < 10);
		requires(x < 15);
		ensuresClass(this->c_ == ___pre.c_ + x);
		ensures(this->c_ == ___pre.c_ + x);

		c_ += x; // imaging this is a complex calculation
		print();
	}

	void Reset() { c_ = 0;}

	void AddWithBug(int x) {
		invariant();
		requires(x < 10);
		ensuresClass(c_ == ___pre.c_ + x);

		// BUG
		c_ += x - 1; // imaging this is a complex calculation
		print();
	}

private:

	int c_;
	// isValid is the invariant function
	bool isValid() { return c_ >= 0;}

	void print() { /*cout << c_ << endl;*/}
};

class CounterNoCopyConstructor {
public:
	explicit CounterNoCopyConstructor(int x): c_(x) {invariant();}

	// if you don't have a copy constructor, you can use ensureClass2 and explicitely capture the member variable 
	void AddWithBug(int x) {
		invariant();
		requires(x < 10);
		ensuresClass2(this->c_, c_ == ___pre2 + x);

		c_ += x - 1; // imaging this is a complex calculation
		print();
	}

private:
	// private copy constructor and =
	CounterNoCopyConstructor(const CounterNoCopyConstructor&);
	CounterNoCopyConstructor& operator = (const CounterNoCopyConstructor&);

	int c_;
	bool isValid() { return c_ >= 0;}
	void print() { /*cout << c_ << endl;*/}
};

BOOST_AUTO_TEST_SUITE(DBCSuite)

BOOST_AUTO_TEST_CASE(DBCTest)
{
	// respects invariant, pre and postcondition for function
	BOOST_CHECK_NO_THROW(divide(10, 3));

	// fails precondition
	BOOST_CHECK_THROW(divide(10, 0), preexception);

	// fails postcondition
	BOOST_CHECK_THROW(divide(-10, 3), postexception);

	// respects invariant, pre and postcondition for class
	Counter c;
	BOOST_CHECK_NO_THROW(c.Add(3));

	// fails invariant
	BOOST_CHECK_THROW(c.Add(-4), postexception);

	// fails precondition
	c.Reset();
	BOOST_CHECK_THROW(c.Add(15), preexception);

	// fails postcondition
	c.Reset();
	BOOST_CHECK_THROW(c.AddWithBug(4), postexception);

	// fails postcondition for class with no copy constructor
	CounterNoCopyConstructor c1(3);
	BOOST_CHECK_THROW(c1.AddWithBug(4), postexception);
}

BOOST_AUTO_TEST_SUITE_END()
