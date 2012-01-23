#ifndef L_FUNCIONAL_HPP
#define L_FUNCIONAL_HPP

#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")

template<class T>
std::ostream& operator<<(std::ostream& s, std::vector<T> const& vec) {
	using namespace std;
	copy(vec.begin(), vec.end(), ostream_iterator<T>(s, " "));
	return s;
}


template<class Range>
auto l_sort(Range r) -> Range {return boost::range::sort(r1);};

template<class Range, class F>
auto l_sort(Range r, F f) -> Range { return boost::range::sort(r, f);};

template<class Container, class Range, class F>
auto l_map(const Range& r, F f) -> Container {
	Container c;
	boost::range::transform(r, std::back_inserter(c), f);
	return c;
}

template<class Range, class T, class F>
auto l_fold(const Range& r, T init, F f) -> T { return boost::accumulate(r, init, f);}

#endif

