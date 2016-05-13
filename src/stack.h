#ifndef MINISTL_STACK_H
#define MINISTL_STACK_H

#include "deque.h"

namespace ministl {

template <typename T, typename Sequence = deque<T>>
class stack {
public:
  typedef typename Sequence::value_type      value_type;
  typedef typename Sequence::pointer         pointer;
  typedef typename Sequence::const_pointer   const_pointer;
  typedef typename Sequence::reference       reference;
  typedef typename Sequence::const_reference const_reference;
  typedef typename Sequence::size_type       size_type;
  typedef typename Sequence::difference_type difference_type;

public:
  stack( ) : container( ) { }
  stack(size_t n) : container(n) { }
  stack(size_t n, const value_type& value) : container(n, value) { }
  ~stack( ) = default;
  bool empty( ) const { return container.empty( ); }
  size_type size( ) const { return container.size( ); }
  reference top( ) { return container.back( ); }
  const_reference top( ) const { return container.back( ); }
  void push(const value_type& x) { container.push_back(x); }
  void pop( ) { container.pop_back( ); }

protected:
  Sequence container;
};

} // namespace ministl

#endif // MINISTL_STACK_H