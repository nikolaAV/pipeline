#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <vector>
#include <cassert>

#include "transform_iterator.hpp"

namespace test_transform_iterator {

namespace alt {

template< typename InputIt, typename OutputIt, typename UnaryOperation >
OutputIt transform(InputIt first, InputIt last, OutputIt out, UnaryOperation op)
{
   auto const converter = stdext::output::make_transformer(op);
   return std::copy(first, last, converter(out)).get_underlying();
}

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
OutputIt transform_if(InputIt first, InputIt last, OutputIt out, UnaryPredicate pred, UnaryOperation op)
{
   auto const converter = stdext::output::make_transformer(op);
   return std::copy_if(first, last, converter(out), pred).get_underlying();
}

} // namespace alt


using namespace std;

void algo_transform()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<double> const expected = { 0.,2.,4.,6.,8.,10.,12.,14.,16.,18. };
   auto const op = [](auto v) {return v * 2.;};
   vector<double> result1, result2;

   std::transform(in.begin(), in.end(), back_inserter(result1), op);
   assert(result1==expected);

   alt::transform(in.begin(), in.end(), back_inserter(result2), op);
   assert(result2==expected);
}

void algo_transform_if()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<double> expected = { 0.,4.,8.,12.,16. };
   auto const op = [](auto v) {return v * 2.; };
   auto const even = [](auto v) {return v % 2 == 0; };
   vector<double> result;

   alt::transform_if(in.begin(), in.end(), back_inserter(result), even, op);
   assert(result == expected);
   copy(begin(result), end(result), ostream_iterator<double>(cout, ","));
   cout << endl;    
}


} // namespace test_transform_iterator

