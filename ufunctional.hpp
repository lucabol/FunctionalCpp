#ifndef L_FUNCIONAL_HPP
#define L_FUNCIONAL_HPP

namespace ufunctional {
	using namespace std;
	using namespace boost::range;

    // Utility functions
	#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")
    #define CHECK_ITEMS(c1,c2) \
        BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(c1),boost::end(c1), boost::begin(c2),boost::end(c2))

	template<class T>
	ostream& operator<<(ostream& s, vector<T> const& vec) {
		copy(vec.begin(), vec.end(), ostream_iterator<T>(s, " "));
		return s;
	}

    template <class T>
    inline auto to_string(T value) -> string {
       stringstream ss;
       ss << value;
       return ss.str();
    }

	template <class T>
	inline auto uidentity(T x) RETURNS( x );

    // Prelude functions
	template<class Range, class F>
	inline auto ufilter(const Range& r, F f) RETURNS( r | filtered(f) );

	template<class Range, class T>
	inline auto ureplace(const Range& r, T oldValue, T newValue)
        RETURNS( r | replaced(oldValue, newValue) );

	template<class Range, class T, class F>
	inline auto ureplace_if(const Range& r, F f,T newValue)
        RETURNS( r | replaced_if(f, newValue) );

	template<class Range, class F>
	inline auto umap2(const Range& r, F f)
        RETURNS( r | transformed(f) );

	// Taking the argoment by value creates a copy that doesn't modify the passed array
	template<class Range>
	inline auto usort(Range r) -> Range {return sort(r);}

	template<class Range, class F>
	inline auto usort(Range r, F f) -> Range { return sort(r, f);}

	template<class Container, class Range, class F>
	inline auto umap(const Range& r, F f) -> Container {
		Container c;
		transform(r, back_inserter(c), f);
		return c;
	}

	template<class Range, class T, class F>
	inline auto ufold(const Range& r, T init, F f) -> T { return boost::accumulate(r, init, f);}

    // Helpers for functional types, i.e. records, tuples, variant, option
    #ifdef __cplusplus
    #define urecord(Record)                                                                      \
           bool operator==(Record& other) const {                                                \
                  static_assert(std::is_trivial<Record>::value, "Not trivially copyable");       \
                  return memcmp(this, &other, sizeof(Record)) == 0;}                             \
           bool operator!=(Record& other) const { return !(*this == other);}
    #else
    #define urecord(Record)
    #endif

    //Function composition
	template< class A, class F1 >
	inline auto pipe(const A & a, const F1 & f1) -> decltype( std::remove_reference< decltype(f1(a)) >::type() ) {
		return f1(a); 
	}

	template< class A, class F1, class F2 >
	inline auto pipe(const A & a, const F1 & f1, const F2 & f2)
		-> decltype( std::remove_reference< decltype(f2(f1(a))) >::type() ) {
		return f2(f1(a)); 
	}

	template< class A, class F1, class F2 >
	inline auto pipe(const A & a, const F1 & f1, const F2 & f2, const F3 & f3)
		-> decltype( std::remove_reference< decltype(f3(f2(f1(a)))) >::type() ) {
		return f3(f2(f1(a))); 
	}

//    template <class A, class B, class C>
//    inline function<C(A)> compose(function<C(B)> f, function<B(A)> g) {
//        return [f,g](A x) { return f(g(x));
//        };
//   }
}

#endif

