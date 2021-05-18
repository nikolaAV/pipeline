#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <vector>
#include <cassert>

#include "transform_iterator.hpp"
#include "filter_iterator.hpp"

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

template< typename InputIt, typename OutputIt, typename UnaryOperation >
OutputIt transform2(InputIt first, InputIt last, OutputIt out, UnaryOperation op)
{
   auto const converter = stdext::input::make_transformer(op);
   for( auto iter = converter(first); iter != last; ++iter) 
      *out++ = *iter;    
   return out;
}

template< typename InputIt, typename OutputIt, typename UnaryOperation >
OutputIt transform3(InputIt first, InputIt last, OutputIt out, UnaryOperation op)
{
   auto const converter = stdext::input::make_transformer(op);
   std::for_each(converter(first), converter(last), [&out](auto const v){
      *out++ = v;
   });
   return out;
}

template< typename InputIt, typename OutputIt, typename UnaryOperation >
OutputIt transform4(InputIt first, InputIt last, OutputIt out, UnaryOperation op)
{
   auto const converter = stdext::input::make_transformer(op);
   return std::copy(converter(first), converter(last), out);
   return out;
}


} // namespace alt


using namespace std;

void algo_transform()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<double> const expected = { 0.,2.,4.,6.,8.,10.,12.,14.,16.,18. };
   auto const op = [](auto v) {return v * 2.;};

   vector<double> result1;
   std::transform(in.begin(), in.end(), back_inserter(result1), op);
   assert(result1==expected);

   vector<double> result2;
   alt::transform(in.begin(), in.end(), back_inserter(result2), op);
   assert(result2==expected);

   vector<double> result3;
   alt::transform2(in.begin(), in.end(), back_inserter(result3), op);
   assert(result3==expected);

   vector<double> result4;
   alt::transform3(in.begin(), in.end(), back_inserter(result4), op);
   assert(result4==expected);

   vector<double> result5;
   alt::transform4(in.begin(), in.end(), back_inserter(result5), op);
   assert(result5==expected);

   copy(begin(result5), end(result5), ostream_iterator<double>(cout, ","));
   cout << endl;    
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
}

} // namespace test_transform_iterator


namespace test_filter_iterator {

template< typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
OutputIt copy_if(InputIt first, InputIt last, OutputIt out, UnaryPredicate pred, UnaryOperation op)
{
   auto const filter = stdext::input::make_transformer(pred);
   return copy(filter(first, last), filter(last, last), out);
}

using namespace std;

void algo_copy_if()
{
   vector<int> const in = { 0,1,2,3,4,5,6,7,8,9 };
   vector<int> expected = { 0,2,4,6,8 };
   auto const even = [](auto v) {return v % 2 == 0; };
   vector<int> result;

   copy_if(in.begin(), in.end(), back_inserter(result), even);
   assert(result == expected);
}

} // namespace test_filter_iterator
