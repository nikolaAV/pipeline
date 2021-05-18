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
      assignable<Function> func_;

   public:
      explicit transform_iterator(Iterator iterator, Function func)
         : iterator_(std::move(iterator))
         , func_(std::move(func))
      {}

      transform_iterator& operator++() { ++iterator_; return *this; }
      transform_iterator& operator*() { return *this; }
      template<typename T>
      transform_iterator& operator=(T const& value)
      {
         *iterator_ = func_.get()(value);
         return *this;
      }
      Iterator get_underlying() const { return iterator_; }
   };

   template<typename Function>
   class transformer
   {
      Function func_;

   public:
      explicit transformer(Function func) : func_(std::move(func)) {}
      template<typename Iterator>
      transform_iterator<Iterator, Function> operator()(Iterator iterator) const
      {
         return transform_iterator<Iterator, Function>(iterator, func_);
      }
   };

   template<typename Function>
   transformer<Function> make_transformer(Function func)
   {
      return transformer<Function>(std::move(func));
   }

}  // namespace output

namespace input {

   /**
      \brief  shifts the transform function into the input iterator.
      \param Iterator   - a type of the input iterator for which into the transform function has to be mixed
      \param Function   - any functional object with signature:
                                       T func(Iterator::value_type)
      \see  ???
   */

   template<typename Iterator, typename Function>
   class transform_iterator
   {
      Iterator iterator_;
      assignable<Function> func_;

   public:
      explicit transform_iterator(Iterator iterator, Function func)
         : iterator_(std::move(iterator))
         , func_(std::move(func))
      {}

      transform_iterator& operator++() { ++iterator_; return *this; }
      decltype(auto) operator*()
      {
         return func_.get()(*iterator_);   
      }
      transform_iterator& operator=(typename Iterator::value_type const& value)
      {
         *iterator_ = value;
         return *this;
      }
      bool operator==(transform_iterator const& other) noexcept 
      {
         return iterator_ == other.iterator_;
      }
      bool operator==(Iterator const& other) noexcept 
      {
         return iterator_ == other;
      }
      bool operator!=(transform_iterator const& other) noexcept 
      {
         return iterator_ != other.iterator_;
      }
      bool operator!=(Iterator const& other) noexcept 
      {
         return iterator_ != other;
      }
      auto operator-(transform_iterator const& other) noexcept 
      {
         return iterator_ - other.iterator_;
      }

      Iterator get_underlying() const { return iterator_; }
   };

   template<typename Function>
   class transformer
   {
      Function func_;

   public:
      explicit transformer(Function func) : func_(std::move(func)) {}
      template<typename Iterator>
      transform_iterator<Iterator, Function> operator()(Iterator iterator) const
      {
         return transform_iterator<Iterator, Function>(iterator, func_);
      }
   };

   template<typename Function>
   transformer<Function> make_transformer(Function func)
   {
      return transformer<Function>(std::move(func));
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
      using difference_type = typename iterator_traits<Iterator>::difference_type;
   };

   template<typename Iterator, typename Function>
   struct iterator_traits<stdext::input::transform_iterator<Iterator, Function>>
   {
      using iterator_category = typename iterator_traits<Iterator>::iterator_category;
      using value_type = typename iterator_traits<Iterator>::value_type;
      using difference_type = typename iterator_traits<Iterator>::difference_type;
   };

} // namespace std

#endif // STD_EXT_TRANSFORM_ITERATOR_INCLUDED_
