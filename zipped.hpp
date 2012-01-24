#ifndef BOOST_RANGE_ADAPTOR_ZIPPED_HPP
#define BOOST_RANGE_ADAPTOR_ZIPPED_HPP

#include "zipped_preprocessor_detail.hpp"

namespace boost
{
  namespace range_detail
  {

      template <typename F>
      struct unpacker
      {
        explicit unpacker(F f) : m_f(f) {}

        template <typename T> BOOST_DEDUCED_TYPENAME DetermineResult<F, T>::type
          operator()(const T& tuple) const
        { return unpack_(mpl::int_<tuples::length<T>::value>(), m_f, tuple); }

        F m_f;
      };//end unpacker

  } // 'range_detail'


  namespace adaptors
  {
    template <typename F> inline range_detail::unpacker<F> unpack(F f)
    { return range_detail::unpacker<F>(f); }

  }//end 'adaptors'
}//end 'boost'

#endif

