#ifndef MINISTL_QUEUE_H
#define MINISTL_QUEUE_H

#include "deque.h"

namespace ministl {

template <typename T, typename Sequence = deque<T>>
class queue {
public:
  typedef typename Sequence::value_type      value_type;
  typedef typename Sequence::pointer         pointer;
  typedef typename Sequence::const_pointer   const_pointer;
  typedef typename Sequence::reference       reference;
  typedef typename Sequence::const_reference const_reference;
  typedef typename Sequence::size_type       size_type;

public:
  queue( ) : container( ) { }
  queue(size_type n) : container(n) { }
  queue(size_type n, const value_type& x) : container(n, x) { }
  bool empty( ) const { return container.empty( ); }
  size_type size( ) const { return container.size( ); }
  reference front( ) { return container.front( ); }
  const_reference front( ) const { return container.front( ); }
  reference back( ) { return container.back( ); }
  const_reference back( ) const { return container.back( ); }
  void push(const value_type& x) { container.push_back(x); }
  void pop( ) { container.pop_front( ); }
  
protected:
  Sequence container;
};


template <typename T, typename Sequence = vector<int>,
  typename Compare = less<typename Sequence::value_type >>
class priority_queue {
public:
  using value_type = typename Sequence::value_type;
  using reference_type = typename Sequence::referenec_type;
  using const_reference_type = typename Sequence::const_reference_type;
  using size_type = typename Sequence::size_type;

public:
  priority_queue( ) : c( ) { }
  explicit priority_queue(const Compare& x) : c( ), comp(x) { }

  template <class InputIterator>
  priority_queue(InputIterator first, InputIterator last)
    : c(first, last)
  {
    make_heap(c.begin( ), c.end( ), comp);
  }

  template <class InputIterator>
  priority_queue(InputIterator first, InputIterator last, const Compare& x)
    : c(first, last), comp(x)
  {
    make_heap(c.begin( ), c.end( ), comp);
  }

  bool empty( ) const { return c.empty( ); }
  size_type size( ) const { return c.size( ); }
  const_reference top( ) const { return c.front( ); }
  void push(const value_type& x)
  {
    try {
      c.push_back(x);
      push_heap(c.begin( ), c.end( ), comp);
    } catch (...) {
      c.clear( );
      throw;
    }
  }

  void pop( )
  {
    try {
      pop_heap(c.begin( ), c.end( ), comp);
      c.pop_back( );
    } catch (...) {
      c.clear( );
      throw;
    }
  }

protected:
  Sequence c;
  Compare comp;
};

} // namespace ministl

#endif // MINISTL_QUEUE_H