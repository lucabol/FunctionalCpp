#ifndef L_FUNCIONAL_HPP
#define L_FUNCIONAL_HPP

using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::range;

#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true,"")

template<class T>
ostream& operator<<(ostream& s, vector<T> const& vec) {
	copy(vec.begin(), vec.end(), ostream_iterator<T>(s, " "));
	return s;
}

struct int_to_string {
	typedef string result_type;

	auto operator()(int number) const -> string {
	   stringstream ss;
	   ss << number * 2;
	   return ss.str();
	}
};

template<class Range>
auto l_sort(Range r) -> Range {return sort(r1);};

template<class Range, class F>
auto l_sort(Range r, F f) -> Range { return sort(r, f);};

template<class Container, class Range, class F>
auto l_map(const Range& r, F f) -> Container {
	Container c;
	transform(r, back_inserter(c), f);
	return c;
}

template<class Range, class T, class F>
auto l_fold(const Range& r, T init, F f) -> T { return accumulate(r, init, f);}

#endif

