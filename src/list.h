#ifndef MINISTL_LIST_H
#define MINISTL_LIST_H

#include "iterator_base.h"
#include "alloc.h"
#include "algorithm.h"
#include "construct.h"
#include <cstddef>

namespace ministl {

// the definition of node
template <typename T>
struct list_node {
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

// the definition of list iterator
template <typename T, typename Ref, typename Ptr>
struct list_iterator {
    typedef list_iterator<T, T&, T*> iterator;
    typedef list_iterator<T, Ref, Ptr> self;

    typedef MiniSTL::bidirectional_iterator_tag iterator_category;
    typedef T                 value_type;
    typedef Ptr               pointer;
    typedef Ref               reference;
    typedef list_node<T>*     link_type;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference;

    link_type node;

    // constructor
    list_iterator( ) { }
    list_iterator(link_type x): node(x) { }
    list_iterator(const iterator &x): node(x.node) { }
    
    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const { return (*node).data; }
    pointer operator->() const  { return &(operator*()); }

    self& operator++()
    {
        node = (link_type)((*node).next);
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--()
    {
        node = (link_type)((*node).prev);
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template <typename T, typename Alloc = alloc>
class list {
protected:
    typedef list_node<T> list_node;
    typedef MiniSTL::simple_alloc<list_node, Alloc> list_node_allocator;
public:
    typedef T                                      value_type;
    typedef value_type*                            pointer;
    typedef const valut_type*                      const_pointer;
    typedef value_type&                            reference;
    typedef const value_type&                      const_reference;
    typedef size_t                                 size_type;
    typedef ptrdiff_t                              difference;
    typedef list_node* link_type;
    typedef list_iterator<T, T&, T*>             iterator;
    typedef list_iterator<T, const T&, const T*> const_iterator;
protected:
    link_type node;
protected:
    link_type get_node( ) { return list_node_allocator::allocate( ); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    link_type create_node(const T& x) 
    {
        link_type p = get_node( );
        MiniSTL::construct(p, x);
        return p;
    }

    void empty_initialize( )
    {
        node = get_node( );
        node->next = node;
        node->prev = node;
    }

    void transfer(iterator position, iterator first, iterator last)
    {
        if (position != last) {
            (*(link_type((*last.node).prev))).next = position.node;
            (*(link_type((*first.node).next))).next = last.node;
            (*(link_type((*position.node).prev))).next = first.node;
            link_type tmp = link_type((*position.node).prev);
            (*position.node)->prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    }

public:
    list( ) { empty_initialize( ); }
    iterator begin( ) { return (link_type)((*node).next); }
    iterator end( )   { return node; }
    reference front() { return *begin; }
    reference back() { return *(--end()); }
    bool empty() const { return node->next = node; }
    size_type size( ) const 
    {
        size_type result = 0;
        MiniSTL::distance(begin( ), end( ), result);
        return result;
    }
    void push_front(const T& x) { insert(begin( ), x); }
    void push_back(const T& x) { insert(end( ), x); }
    iterator insert(iterator position, const T &x) 
    {
        link_type tmp = create_node(x);

        tmp->next = position.node;
        tmp->prev = position.node->prev;
        (link_type(position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }

    void pop_front( ) { erase(begin( )); }
    void pop_back( ) 
    {
        iterator tmp = end( );
        erase(--tmp);
    }
    
    iterator erase(iterator position)
    {
        link_type next_node = link_type(position.node->next);
        link_type prev_node = link_type(position.node->prev);

        prev_node->next = next_node;
        next_node->prev = prev_node;
        MiniSTL::destory(position.node);
        return iterator(next_node);
    }

    void clear( );
    void remove(const T &x);
    void unique( );

 };

template <typename T, typename Alloc>
void list<T, Alloc>::clear( )
{
    link_type cur = (link_type)node->next;

    while (cur != node) {
        link_type tmp = cur;
        cur = (link_type)cur->next;
        list_node_allocator::deallocate(tmp);
    }
    node->next = node;
    node->prev = node;
}

template <typename T, typename Alloc>
void list<T, Alloc>::remove(const T& value)
{
    iterator first = begin( );
    iterator last = end( );

    while (first != last) {
        iterator next = first;
        ++next;
        if (*first == value)
            erase(first);
        first = next;
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::unique( )
{
    iterator first = begin( );
    iterator last = end( );

    if (first == last)
        return;
    iterator next = first;
    while (++next != last) {
        if (*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

} // namespace ministl

#endif // MiniSTL