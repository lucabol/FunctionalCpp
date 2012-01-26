#ifndef MAPPED_HPP
#define MAPPED_HPP

// from http://smellegantcode.wordpress.com/2011/10/31/linq-to-c-or-something-much-better/

#include <pstade/oven/transformed.hpp>

#include <boost/lexical_cast.hpp>

#include <vector>
#include <iostream>
#include <string>

namespace functional {

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

	template <class F>
	decltype(pstade::oven::transformed(functor_with_traits(make_ref<F>()))) mapped(F f)
	{
		return pstade::oven::transformed(functor_with_traits(f)); 
	}

	#define mappedL transformed

}
#endif