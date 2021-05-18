#ifndef STD_EXT_FILTER_ITERATOR_INCLUDED_
#define STD_EXT_FILTER_ITERATOR_INCLUDED_

#include <utility>
#include <iterator>
#include <algorithm>

#include "assignable.hpp"

namespace stdext {

namespace input {

   /**
      \brief  shifts the filter function into the input iterator.
      \param Iterator   - a type of the input iterator for which into the filter function has to be mixed
      \param Predicate  - any functional object with signature:
                                       T func(Iterator::value_type)
      \see  ???
   */

   template<typename Iterator, typename Predicate>
   class filter_iterator
   {
      Iterator iterator_;
      Iterator end_;
      assignable<Predicate> pred_;

   public:
      explicit filter_iterator(Iterator first, Iterator last, Predicate pred)
         : iterator_(std::move(first))
         , end_(std::move(last))
         , pred_(std::move(pred))
      {}

      filter_iterator& operator++() 
      {  
         ++iterator_; 
         iterator_ = std::find_if(iterator_, end_, pred_.get());
         return *this; 
      }

      decltype(auto) operator*()
      {
         return *iterator_;   
      }
      bool operator==(filter_iterator const& other) noexcept 
      {
         return iterator_ == other.iterator_;
      }
      bool operator==(Iterator const& other) noexcept 
      {
         return iterator_ == other;
      }
      bool operator!=(filter_iterator const& other) noexcept 
      {
         return iterator_ != other.iterator_;
      }
      bool operator!=(Iterator const& other) noexcept 
      {
         return iterator_ != other;
      }

      Iterator get_underlying() const { return iterator_; }
   };

   template<typename Predicate>
   class filter
   {
      Predicate pred_;

   public:
      explicit filter(Predicate pred) : pred_(std::move(pred)) {}
      template<typename Iterator>
      filter_iterator<Iterator, Predicate> operator()(Iterator iterator) const
      {
         return filter_iterator<Iterator, Predicate>(iterator, pred_);
      }
   };

   template<typename Predicate>
   filter<Predicate> make_filter(Predicate pred)
   {
      return filter<Predicate>(std::move(pred));
   }

}  // namespace output



}  // namespace stdext

namespace std
{
   template<typename Iterator, typename Predicate>
   struct iterator_traits<stdext::input::filter_iterator<Iterator, Predicate>>
   {
      using iterator_category = typename iterator_traits<Iterator>::iterator_category;
      using value_type = typename iterator_traits<Iterator>::value_type;
      using difference_type = typename iterator_traits<Iterator>::difference_type;
   };

} // namespace std

#endif // STD_EXT_FILTER_ITERATOR_INCLUDED_
