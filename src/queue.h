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

} // namespace ministl

#endif // MINISTL_QUEUE_H