#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <bdlist.hpp>

namespace ishmuratov
{
  template< class T >
  class Queue
  {
    public:
      void push(const T & rhs)
      {
        c_.pushFront(rhs);
      }

      T drop()
      {
        T value = c_.back();
        c_.popBack();
        return value;
      }

      T & front()
      {
        return c_.front();
      }

      T & back()
      {
        return c_.back();
      }

      bool empty() const
      {
        return c_.empty();
      }

      size_t size()
      {
        return c_.size();
      }

    private:
      List< T > c_;
  };
}

#endif
