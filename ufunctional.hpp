#ifndef L_FUNCIONAL_HPP
#define L_FUNCIONAL_HPP

#include <cstring>
#include <type_traits>
#include <pstade/oven/transformed.hpp>

namespace functional {
	using namespace std;
	using namespace pstade::oven;

    // Utility functions
	#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")
    #define CHECK_ITEMS(c1,c2) \
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(c1), end(c1), begin(c2), end(c2))


    // Helpers for functional types, i.e. records, tuples, variant, option
    #ifdef __cplusplus
    #define RECORD(Record)                                                                       \
           bool operator==(Record& other) const {                                                \
                  static_assert(std::is_trivial<Record>::value, "Not trivially copyable");       \
                  return memcmp(this, &other, sizeof(Record)) == 0;}                             \
           bool operator!=(Record& other) const { return !(*this == other);}
    #else
    #define RECORD(Record)
    #endif

    //Function composition
	template< class A, class F1 >
	inline auto pipe(const A & a, const F1 & f1) -> decltype( remove_reference< decltype(f1(a)) >::type() ) {
		return f1(a); 
	}

	template< class A, class F1, class F2 >
	inline auto pipe(const A & a, const F1 & f1, const F2 & f2)
		-> decltype( remove_reference< decltype(f2(f1(a))) >::type() ) {
		return f2(f1(a)); 
	}

	template< class A, class F1, class F2, class F3 >
	inline auto pipe(const A & a, const F1 & f1, const F2 & f2, const F3 & f3)
		-> decltype( remove_reference< decltype(f3(f2(f1(a)))) >::type() ) {
		return f3(f2(f1(a))); 
	}

	// Add result_type to a normal function or lambda
	// Maybe some of these subfunctions are already in boost somewhere
	// from http://smellegantcode.wordpress.com/2011/10/31/linq-to-c-or-something-much-better/

	template <class T>
	T *make_ptr() { return (T *)0; }

	template <class T>
	const T &make_ref() { return *(make_ptr<T>()); }

	template <class T>
	struct mem_fun_traits { };

	template <class R, class C, class A>
	struct mem_fun_traits<R (C::*)(A) const> 
	{
	  typedef R result_type;
	  typedef A argument_type;
	};

	template<typename T>
	struct functor_member {
		typedef decltype(&T::operator()) type;
	};

	template <class F>
	struct functor_traits
	{
		typedef typename functor_member<F>::type member_t;
		typedef mem_fun_traits<member_t> traits;

		typedef typename traits::result_type result_type;
		typedef typename traits::argument_type argument_type;
	};

	template <class F>
	class functor_with_traits_t
	{
		F f;

	public:
		functor_with_traits_t(F f_) : f(f_) {}

		typedef typename functor_traits<F>::result_type result_type;
		typedef typename functor_traits<F>::argument_type argument_type;

		result_type operator()(argument_type a) const { return f(a); }
	};

	template <class F>
	functor_with_traits_t<F> functor_with_traits(F f) { return functor_with_traits_t<F>(f); }

	// This function works with lambdas and normal functions (transformed doesn't)
	template <class F>
	decltype(pstade::oven::transformed(functor_with_traits(make_ref<F>()))) mapped(F f)
	{
		return pstade::oven::transformed(functor_with_traits(f)); 
	}
}

#endif

