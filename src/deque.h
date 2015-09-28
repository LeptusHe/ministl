#ifndef MINISTL_DEQUE_H
#define MINISTL_DEQUE_H

#include <cstddef>
#include "alloc.h"
#include "iterator_base.h"

using namespace ministl;
// namespace ministl {

inline size_t deque_buf_size(size_t size)
{
    return size < 512 ? size_t(512 / size) : size_t(1);
}

template <typename Tp, typename Ref, typename Ptr, size_t BufSize>
struct Deque_iterator {
    typedef Deque_iterator<Tp, Tp&, Tp*>              iterator;
    typedef Deque_iterator<Tp, const Tp&, const Tp*>  const_iterator;
    static  size_t S_buffer_size( ) { return deque_buf_size(sizeof(T)); }

    typedef random_access_iterator_tag iterator_category;
    typedef Tp         value_type;
    typedef Ptr        pointer;
    typedef Ref        reference;
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef Tp**       Map_pointer;

    typedef Deque_iterator Self;

    Tp* cur;
    Tp* first;
    Tp* last;
    Map_pointer node;

    Deque_iterator(Tp *x, Map_pointer y)
        :cur(x), first(*y),
        last(*y + S_buffer_size( )), node(y) { }
    Deque_iterator( ): cur(nullptr), first(nullptr), last(nullptr), node(nullptr) { }
    Deque_iterator(const iterator &x)
        :cur(x.cur), first(x.first), last(x.last),
        node(x.node) { }

    void set_node(Map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + difference_type(S_buffer_size( ));
    }

    reference operator*() { return *cur; }
    pointer operator->() const { return cur; }
    difference_type operator-(const Self& x) const
    {
        return difference_type(S_buffer_size( )) * (node - x.node - 1)
            + (cur - first) + (x.last - x.cur);
    }

    Self& operator++()
    {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
    }

    Self operator++(int)
    {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    Self& operator--()
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

        if (offset >= 0 && offset < difference_type(S_buffer_size( )))
            cur += n;
        else {
            difference_type node_offset = offset > 0 ? offset / difference_type(S_buffer_size( ))
                : -difference_type((-offset - 1) / S_buffer_size( )) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(S_buffer_size( )));
        }
        return *this;
    }

    Self operator+(difference_type n) const
    {
        Self tmp = *this;
        return tmp += n;
    }

    Self& operator-=(difference_type n) { return *this += -n; }
    Self  operator-(difference_type n) const 
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
        return (node == x.node) ? (cur > x.cur) : ()
    }

    bool operator<=(const Self& x) const
    {
        return x < *this;
    }

    bool operator>=(const Self& x) const
    {
        return !(*this < x);
    }
};


template <typename T, typename Alloc = alloc, size_t BufSiz = 0>
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
    typedef Deque_iterator<T, T&, T*, BufSiz> iterator;

protected:
    typedef pointer* map_pointer;

protected:
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size;
protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    typedef simple_alloc<pointer, Alloc> map_allocator;

public:
    iterator begin( ) { return start; }
    iterator end( ) { return finish; }
    reference operator[](size_type n)
    {
        return start[difference_type(n)];
    }

    reference front( ) { return *start; }
    reference back( ) 
    {
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }

    size_type size( ) const { return finish - start; }
    size_type max_size( ) const { return size_type(-1); }
    bool empty( ) const { return finish == start; }

};
// } // namespace ministl

#endif // MINISTL_DEQUE_H