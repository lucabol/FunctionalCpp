#ifndef ULAZYLIST_HPP_INCLUDED
#define ULAZYLIST_HPP_INCLUDED

namespace ufunctional {
    using namespace std;

    // Idea taken from http://nonchalantlytyped.net/blog/category/programming/c-programming/
    template <class T>
    struct ufrom {
        T first;
        function<ufrom()> rest;
        ufrom(T firstValue, function<T(T)> f):
            first(firstValue), rest([=] () -> ufrom {return ufrom(f(firstValue), f);}) {}
    };


    template<class T>
    vector<T> utake(int n, ufrom<T> f) {
        if (n==0)
            return vector<T>();
        else
        {
            vector<T> retval;
            retval.push_back(f.first);
            vector<T> temp = utake(n-1, f.rest());
            std::copy(temp.begin(), temp.end(),
                      std::inserter(retval, retval.end()));
            return retval;
        }
    }
}

#endif // ULAZYLIST_HPP_INCLUDED
