#ifndef L_FUNCIONAL_HPP
#define L_FUNCIONAL_HPP

namespace ufunctional {
	using namespace std;
	using namespace boost::range;

	#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")
	#define BENDL(v) v.begin(), v.end()

	template<class T>
	ostream& operator<<(ostream& s, vector<T> const& vec) {
		copy(vec.begin(), vec.end(), ostream_iterator<T>(s, " "));
		return s;
	}

	template <class T>
	auto uidentity(T x) RETURNS( x );

	template<class Range, class F>
	auto ufilter(const Range& r, F f) RETURNS( r | filtered(f) );

	template<class Range>
	auto usort(Range r) -> Range {return sort(r);}

	template<class Range, class F>
	auto usort(Range r, F f) -> Range { return sort(r, f);}

	template<class Container, class Range, class F>
	auto umap(const Range& r, F f) -> Container {
		Container c;
		transform(r, back_inserter(c), f);
		return c;
	}

	template<class Range, class T, class F>
	auto ufold(const Range& r, T init, F f) -> T { return boost::accumulate(r, init, f);}

}

#endif

