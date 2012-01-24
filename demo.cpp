
#include <boost/assign/list_of.hpp>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/adaptor/zipped.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/irange.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <iostream>
#include <vector>

using boost::adaptors::strided;
using boost::adaptors::unpack;
using boost::adaptors::zip;
using boost::assign::list_of;
using boost::get;
namespace px = boost::phoenix;
using px::arg_names::arg1;
using px::arg_names::arg2;
using px::arg_names::arg3;

int main()
{
  const std::vector<int> vec1 = list_of(1)(2)(3)(4)(5)(6);
  std::vector<int> vec2 = list_of(10)(20)(30)(40)(50)(60);
  std::vector<int> vec3 = list_of(100)(200)(300)(400)(500)(600);

  //Make sure we can accept rvalues with zip.
  boost::for_each(zip(vec1 | strided(2), vec2 | strided(2), vec3 | strided(2)),
      unpack(std::cout << px::val('(') << arg1 <<", "<< arg2 << ", " << arg3
               << ")\n"));
  
  //We can zip output iterators too:
  boost::transform(vec1, boost::begin(zip(vec3, vec2)),
     px::construct<boost::tuple<int,int> >(arg1 + 1, arg1 + 2));

  boost::transform(zip(vec1, vec3), boost::begin(vec2),
      unpack(arg1 + arg2));

  //An easier way to work with indexes this way than the current indexed adaptor
  boost::for_each(zip(vec2, boost::irange(0,6)), unpack(
    std::cout << px::val("Index ") << arg2 << ": " << arg1 << std::endl));
}

