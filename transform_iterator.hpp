#ifndef STD_EXT_TRANSFORM_ITERATOR_INCLUDED_
#define STD_EXT_TRANSFORM_ITERATOR_INCLUDED_

#include <utility>
#include <iterator>

#include "assignable.hpp"

namespace stdext {

namespace output {

   /**
      \brief  shifts the transform function into the output iterator.
      \param Iterator   - a type of the output iterator for which into the transform function has to be mixed
      \param Function   - any functional object with signature:
                                       Iterator::value_type func(T)
      \see  https://www.fluentcpp.com/2017/11/28/output-iterator-adaptors-symmetry-range-adaptors/
   */

   template<typename Iterator, typename Function>
   class transform_iterator
   {
      Iterator iterator_;
      assignable<Function> function_;

   public:
      explicit transform_iterator(Iterator iterator, Function function)
         : iterator_(std::move(iterator))
         , function_(std::move(function))
      {}

      transform_iterator& operator++() { ++iterator_; return *this; }
      transform_iterator& operator*() { return *this; }
      template<typename T>
      transform_iterator& operator=(T const& value)
      {
         *iterator_ = function_.get()(value);
         return *this;
      }
      Iterator get_underlying() const { return iterator_; }
   };

   template<typename Function>
   class transformer
   {
      Function function_;

   public:
      explicit transformer(Function function) : function_(function) {}
      template<typename Iterator>
      transform_iterator<Iterator, Function> operator()(Iterator iterator) const
      {
         return transform_iterator<Iterator, Function>(iterator, function_);
      }
   };

   template<typename Function>
   transformer<Function> make_transformer(Function function)
   {
      return transformer<Function>(function);
   }

}  // namespace output

}  // namespace stdext

namespace std
{
   template<typename Iterator, typename Function>
   struct iterator_traits<stdext::output::transform_iterator<Iterator, Function>>
   {
      using iterator_category = typename iterator_traits<Iterator>::iterator_category;
      using value_type = typename iterator_traits<Iterator>::value_type;
   };

} // namespace std

#endif // STD_EXT_TRANSFORM_ITERATOR_INCLUDED_
