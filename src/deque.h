#ifndef MINISTL_DEQUE_H
#define MINISTL_DEQUE_H

#include "alloc.h"
#include "algorithm.h"
#include "iterator_base.h"
#include "uninitialized.h"

namespace ministl {

inline size_t DequeBufSize(size_t size)
{
  return size < 512 ? size_t(512 / size) : size_t(1);
}

template <typename Tp, typename Ref, typename Ptr, size_t BufSize>
struct DequeIterator {
  typedef DequeIterator<Tp, Tp&, Tp*, BufSize>              iterator;
  typedef DequeIterator<Tp, const Tp&, const Tp*, BufSize>  const_iterator;

  typedef random_access_iterator_tag iterator_category;
  typedef Tp         value_type;
  typedef Ptr        pointer;
  typedef Ref        reference;
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;
  typedef Tp**       map_pointer;
  typedef DequeIterator Self;

  Tp* cur;
  Tp* first;
  Tp* last;
  map_pointer node;
  
  static  size_t SBufferSize( ) { return DequeBufSize(sizeof(Tp)); }

  DequeIterator( ) : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) { }
  DequeIterator(Tp *x, map_pointer y)
    :cur(x), first(*y), last(*y + SBufferSize( )), node(y)
  { }
  DequeIterator(const iterator &x)
    :cur(x.cur), first(x.first), last(x.last), node(x.node)
  { }

  void set_node(map_pointer new_node)
  {
    node = new_node;
    first = *new_node;
    last = first + difference_type(SBufferSize( ));
  }

  reference operator*( ) const { return *cur; }
  pointer operator->( ) const { return cur; }
  difference_type operator-(const Self& x) const
  {
    return difference_type(SBufferSize( )) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
  }

  Self& operator++( )
  {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }

  Self operator++(int)
  {
    Self tmp = *this;
    ++*this;
    return tmp;
  }

  Self& operator--( )
  {
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }

  Self operator--(int)
  {
    Self tmp = *this;
    --*this;
    return tmp;
  }

  Self& operator+=(difference_type n)
  {
    difference_type offset = n + (cur - first);

    if (offset >= 0 && offset < difference_type(SBufferSize( )))
      cur += n;
    else {
      difference_type node_offset = offset > 0 ? offset / difference_type(SBufferSize( ))
        : -difference_type((-offset - 1) / SBufferSize( )) - 1;
      set_node(node + node_offset);
      cur = first + (offset - node_offset * difference_type(SBufferSize( )));
    }
    return *this;
  }

  Self operator+(difference_type n) const
  {
    Self tmp = *this;
    return tmp += n;
  }

  Self& operator-=(difference_type n)
  {
    return *this += -n;
  }

  Self operator-(difference_type n) const
  {
    Self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const
  {
    return *(*this + n);
  }

  bool operator==(const Self& x) const
  {
    return cur == x.cur;
  }

  bool operator!=(const Self& x) const
  {
    return !(*this == x);
  }

  bool operator<(const Self& x) const
  {
    return (node == x.node) ? (cur < x.cur) : (node < x.node);
  }

  bool operator>(const Self& x) const
  {
    return x < *this;
  }

  bool operator<=(const Self& x) const
  {
    return !(x < *this);
  }

  bool operator>=(const Self& x) const
  {
    return !(*this < x);
  }
};


template <typename T, typename Alloc = alloc, size_t BufSize = 0>
class deque {
public:
  typedef T                  value_type;
  typedef value_type*        pointer;
  typedef const value_type*  const_pointer;
  typedef value_type&        reference;
  typedef const value_type&  const_reference;
  typedef size_t             size_type;
  typedef ptrdiff_t          difference_type;

public:
  typedef DequeIterator<T, T&, T*, BufSize> iterator;
  typedef DequeIterator<T, const T&, const T*, BufSize> const_iterator;

protected:
  typedef pointer* map_pointer;
  typedef simple_alloc<value_type, Alloc> data_allocator;
  typedef simple_alloc<pointer, Alloc> map_allocator;

protected:
  static  size_t SBufferSize( ) { return DequeBufSize(sizeof(T)); }

public:
  deque( )
  {
    initialize_map(0);
  }

  deque(size_t n)
  {
    initialize_map(n);
    fill_initialize(value_type( ));
  }

  deque(size_t n, const value_type& value)
  {
    initialize_map(n);
    fill_initialize(value);
  }
  ~deque( )
  {
    if (map) {
      destory_nodes(start.node, finish.node + 1);
      deallocate_map(map, map_size);
    }
  }

  iterator begin( ) { return start; }
  iterator end( ) { return finish; }
  const_iterator begin( ) const { return start; }
  const_iterator end( ) const { return finish; }
  reference front( ) { return *start; }
  const_reference front( ) const { return *start; }
  reference back( )
  {
    iterator tmp = finish;
    --tmp;
    return *tmp;
  }
  const_reference back( ) const
  {
    iterator tmp = finish;
    --tmp;
    return *tmp;
  }

  size_type size( ) const { return finish - start; }
  size_type max_size( ) const { return size_type(-1); }
  bool empty( ) const { return finish == start; }
  reference operator[](size_type n)
  {
    return start[difference_type(n)];
  }

  void push_front(const value_type& value)
  {
    if (start.cur != start.first) {
      construct(start.cur - 1, value);
      --start.cur;
    } else
      push_front_aux(value);
  }

  void push_back(const value_type& value)
  {
    if (finish.cur != finish.last - 1) {
      construct(finish.cur, value);
      ++finish.cur;
    } else
      push_back_aux(value);
  }

protected:
  enum { InitialMapSize = 8 };

protected:
  void push_front_aux(const value_type& value);
  void push_back_aux(const value_type& value);
  pointer allocate_node( ) { return data_allocator::allocate(DequeBufSize(sizeof(T))); }
  void deallocate_node(pointer p) { data_allocator::deallocate(p, DequeBufSize(sizeof(T))); }
  map_pointer allocate_map(size_t n)
  {
    return map_allocator::allocate(n);
  }

  void deallocate_map(map_pointer p, size_t n)
  {
    map_allocator::deallocate(p, n);
  }
  void initialize_map(size_t num_elements);
  void create_nodes(map_pointer nstart, map_pointer nfinish);
  void destory_nodes(map_pointer nstart, map_pointer nfinish);
  void fill_initialize(const value_type& value);
  void reserve_map_at_front(size_type nodes_to_add = 1)
  {
    if (nodes_to_add > size_type(start.node - map))
      reallocate_map(nodes_to_add, true);
  }

  void reserve_map_at_back(size_type nodes_to_add = 1)
  {
    if (nodes_to_add + 1 > map_size - (finish.node - map)) {
      reallocate_map(nodes_to_add, false);
    }
  }

  void reallocate_map(size_type node_to_add, bool add_at_front);

protected:
  iterator start;
  iterator finish;
  map_pointer map;
  size_type map_size;
};


template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::push_front_aux(const value_type& value)
{
  value_type value_copy = value;
  reserve_map_at_front( );
  *(start.node - 1) = allocate_node( );
  try {
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    construct(start.cur, value_copy);
  } catch (...) {
    start.set_node(start.node + 1);
    start.cur = start.first;
    deallocate_node(*(start.node - 1));
    throw;
  }
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::push_back_aux(const value_type& value)
{
  value_type value_copy = value;
  reserve_map_at_back( );
  *(finish.node + 1) = allocate_node( );
  try {
    construct(finish.cur, value_copy);
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;
  } catch (...) {
    deallocate_node(*(finish.node + 1));
    throw;
  }
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::create_nodes(map_pointer nstart, map_pointer nfinish)
{
  map_pointer cur;
  try {
    for (cur = nstart; cur < nfinish; ++cur)
      *cur = allocate_node( );
  } catch (...) {
    destory_nodes(nstart, cur);
    throw;
  }
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::destory_nodes(map_pointer nstart, map_pointer nfinish)
{
  for (map_pointer n = nstart; n < nfinish; ++n)
    deallocate_node(*n);
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::initialize_map(size_t num_elements)
{
  size_type num_nodes = num_elements / SBufferSize() + 1;
  map_size = max(size_type(InitialMapSize), size_type(num_nodes + 2));
  map = map_allocator::allocate(map_size);
  map_pointer nstart = map + (map_size - num_nodes) / 2;
  map_pointer nfinish = nstart + num_nodes;

  try {
    create_nodes(nstart, nfinish);
  } catch (...) {
    deallocate_map(map, map_size);
    map = nullptr;
    map_size = 0;
    throw;
  }
  start.set_node(nstart);
  finish.set_node(nfinish - 1);
  start.cur = start.first;
  finish.cur = finish.first + num_elements % DequeBufSize(sizeof(Tp));
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::fill_initialize(const value_type& value)
{
  map_pointer cur;
  try {
    for (cur = start.node; cur < finish.node; ++cur)
      ministl::uninitialized_fill(*cur, *cur + SBufferSize( ), value);
    ministl::uninitialized_fill(finish.first, finish.cur, value);
  } catch (...) {
    destory(start, iterator(*cur, cur));
  }
}

template <typename Tp, typename Alloc, size_t BufSize>
void deque<Tp, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
  size_type old_num_nodes = finish.node - start.node + 1;
  size_type new_num_nodes = old_num_nodes + nodes_to_add;
  map_pointer new_nstart;
  
  if (map_size > 2 * new_num_nodes) {
    new_nstart = map + (map_size - new_num_nodes) / 2
      + (add_at_front ? nodes_to_add : 0);
    if (new_nstart < start.node)
      ministl::copy(start.node, finish.node + 1, new_nstart);
    else
      ministl::copy_backward(start.node, finish.node + 1,
                    new_nstart + old_num_nodes);
  } else {
    size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
    map_pointer new_map = allocate_map(new_map_size);
    new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
    ministl::copy(start.node, finish.node + 1, new_nstart);
    deallocate_map(map, map_size);
    map = new_map;
    map_size = new_map_size;
  }
  start.set_node(new_nstart);
  finish.set_node(new_nstart + old_num_nodes - 1);
}

} // namespace ministl

#endif // MINISTL_DEQUE_H