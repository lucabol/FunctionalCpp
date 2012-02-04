#ifndef DBC_HPP_
#define DBC_HPP_

#include <functional>
#include <string>
#include <exception>
#include <sstream>

// base class for all exception classes in the library, saves file, line number and message
class dbcexception: public std::exception {
public:
	const char* what() const throw()
	{
		using namespace std;

		// best way to convert an int to a string in portable c++?
		stringstream ss;
		ss << _line;

		static char buffer[100];

#ifdef _MSC_VER
		// uses the safe _Copy_s on msvc
		string s(_file + " at line " + ss.str() + " " + _message);
		auto length = s._Copy_s(buffer, 100, s.length());
#else
		auto length = (_file + " at line " + ss.str() + " " + _message).copy(buffer, 100);
		buffer[length] = '\0';
#endif
		return buffer;
	}

protected:
	dbcexception(const std::string& file, int line, const std::string& message):
		_file(file), _line(line), _message(message) {}
	~dbcexception() throw() {}

private:
	std::string _file;
	int _line;
	std::string _message;
};

// postcondition exception
class postexception: public dbcexception {
public:
	postexception(const std::string& file, int line, const std::string& message):
		dbcexception(file, line, message) {}
};

// precondition exception
class preexception: public dbcexception {
public:
	preexception(const std::string& file, int line, const std::string& message):
		dbcexception(file, line, message) {}
};

// invariantexception
class invariantexception: public dbcexception {
public:
	invariantexception(const std::string& file, int line, const std::string& message):
		dbcexception(file, line, message) {}
};

typedef std::function<void ()> ___dbcLambda;

// stores a lambda and executes it in the destructor, this is the main trick to get postcondition
class ___post {
public:
	___post(const ___dbcLambda& postF): _f(postF) {}
	~___post() { _f();}
private:
	const ___dbcLambda _f;
};

// used to merge the argument of two macros *after* expansion
#define ___MERGE(a, b) a##b

// you can disable pre, post or invariant by enabling the appropriate macro (i.e. disable post, invariant in release)
#ifndef ___POSTCOND

// generates unique names for temporary variable injected in the locals, the indirection is needed to substitute after expansion
// variables need to be unique to allow using multiple clauses in the same function
#define ___POST(a) ___MERGE(___postcond,a)
#define ___UNIQUE_POST ___POST(__LINE__)

#define ___POST2(a) ___MERGE(___postcond2, a)
#define ___UNIQUE_POST2 ___POST2(__LINE__)

#define ___LINE(a) ___MERGE(___line, a)
#define ___UNIQUE_LINE ___LINE(__LINE__)

// ensures is to be used in freestanding functions or after a ensureClass/ensureClass2 call to access pre or pre2
// ensures works by injecting a local that calls the lambda at destruction time
// notice the need to check for uncaught_exception to make sure we are not throwing during a stack unwind called by another throw
// the lambda captures variables, so it will be compiled as a class, hence it is relatively expensive
// if perf is an issue, disable the check in release
// the assignment in the first enures line below is woraround for preprocessor bug in msvc
// http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/2c4698e1-8159-44fc-a64c-d15220acedb8
#define ensures(F) \
	int ___UNIQUE_LINE = __LINE__; \
	auto ___UNIQUE_POST = ___post([&]() {if(!std::uncaught_exception() && !(F)) throw postexception(__FILE__, ___UNIQUE_LINE,"Post-condition failure: " #F);});

// ensuresClass works for classes that have a copy constructor, which will be called to initialize the pre variable
// with the state of the object at the start of a method
// this allows the syntax this.x < pre.x
#define ensuresClass(F) \
	auto ___pre(*this); \
	auto ___UNIQUE_POST = ___post([&]() {if(!std::uncaught_exception() && !(F)) throw postexception(__FILE__, __LINE__,"Post-condition failure: " #F);});

// works when the class has no copy constructor. ASS gets assigned to __pre2.
#define ensuresClass2(ASS,F) \
	auto ___pre2(ASS); \
	auto ___UNIQUE_POST2 = ___post([&]() {if(!std::uncaught_exception() && !(F)) throw postexception(__FILE__, __LINE__,"Post-condition failure: " #ASS " is ___pre2 in " #F);});

#else
#define ensures(F)
#endif

// a precondition is just a normal if statement, so it doesn't impact perf more than a normal check
#ifndef ___PRECOND
#define requires(F) {if((!(F))) throw preexception(__FILE__, __LINE__,"Pre-condition failure: " #F);};
#else
#define requires(F)
#endif

#ifndef ___INVARIANT

#define ___INV(a) ___MERGE(___inv, a)
#define ___UNIQUE_INV ___INV(__LINE__)

// an invariant is both a precondition and a postcondition that call the isValid member function
// you get a compile time error if you don't have such function (good)
#define invariant() \
	if(!(this->isValid())) throw invariantexception(__FILE__, __LINE__,"Invariant failure"); \
	auto ___UNIQUE_INV = ___post([&]() {if((!std::uncaught_exception() && !this->isValid())) throw invariantexception(__FILE__, __LINE__, "Invariant failure");});
#else
#define invariant()
#endif


#endif /*DBC_HPP_*/
