#ifndef MINISTL_RB_TREE_H
#define MINISTL_RB_TREE_H

#include "alloc.h"
#include "construct.h"
#include "iterator_base.h"
#include "pair.h"

namespace ministl {

using RbTreeColorType = bool;
const RbTreeColorType S_rb_tree_red = false;
const RbTreeColorType S_rb_tree_black = true;

struct RbTreeNodeBase {
  using ColorType = RbTreeColorType;
  using BasePtr = RbTreeNodeBase*;

  ColorType color_;
  BasePtr parent_;
  BasePtr left_, right_;

  static BasePtr Minimun(BasePtr x)
  {
    while (x->left_ != nullptr)
      x = x->left_;
    return x;
  }

  static BasePtr Maximun(BasePtr x)
  {
    while (x->right_ != nullptr)
      x = x->right_;
    return x;
  }
};


template <typename ValueType>
struct RbTreeNode : public RbTreeNodeBase {
  using LinkType = RbTreeNode<ValueType>*;

  ValueType value_field_;
};


struct RbTreeIteratorBase {
  using BasePtr = RbTreeNodeBase::BasePtr;
  using iterator_category = bidirectional_iterator_tag;
  using difference_type = ptrdiff_t;

  BasePtr node_;

  void Increment( )
  {
    if (node_->right_ != nullptr) {
      node_ = node_->right_;
      while (node_->left_ != nullptr)
        node_ = node_->left_;
    } else {
      BasePtr y = node_->parent_;
      while (y->right_ == node_) {
        node_ = y;
        y = y->parent_;
      }
      if (node_->right_ != y)
        node_ = y;
    }
  }

  void Decrement( )
  {
    if (node_->color_ == S_rb_tree_red &&
        node_->parent_->parent_ == node_) {
      node_ = node_->right_;
    } else if (node_->left_ != 0) {
      BasePtr y = node_->left_;
      while (y->right_ != nullptr)
        y = y->right_;
      node_ = y;
    } else {
      BasePtr y = node_->parent_;
      while (node_ != y->left_) {
        node_ = y;
        y = y->parent_;
      }
      node_ = y;
    }
  }
};


template <typename ValueType, typename Ref, typename Ptr>
struct RbTreeIterator : public RbTreeIteratorBase {
  using value_type = ValueType;
  using reference = Ref;
  using pointer = Ptr;
  using iterator = RbTreeIterator<ValueType, ValueType&, ValueType*>;
  using const_iterator = RbTreeIterator<ValueType, const ValueType&, const ValueType*>;
  using self = RbTreeIterator<ValueType, Ref, Ptr>;
  using link_type = RbTreeNode<ValueType>*;

  RbTreeIterator( ) { }
  RbTreeIterator(link_type x) { node_ = x; }
  RbTreeIterator(const iterator& iter) { node_ = iter.node_; }

  reference operator*( ) const
  {
    return link_type(node)->value_field_;
  }

  pointer operator->( ) const
  {
    return &(operator*( ));
  }

  self& operator++( )
  {
    Increment( );
    return *this;
  }

  self operator++(int)
  {
    self tmp = *this;
    Increment( );
    return tmp;
  }

  self& operator--( )
  {
    Decrement( );
    return *this;
  }

  self operator--(int)
  {
    self tmp = *this;
    Decrement( );
    return tmp;
  }
};


template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class rb_tree {
protected:
  using void_pointer = void*;
  using base_ptr = RbTreeNodeBase*;
  using rb_tree_node = RbTreeNode<Value>;
  using rb_tree_node_allocator = simple_alloc<rb_tree_node, Alloc>;
  using color_type = RbTreeColorType;

public:
  using key_type = Key;
  using value_type = Value;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using link_type = rb_tree_node*;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

public:
  using iterator = RbTreeIterator<value_type, reference, pointer>;

public:
  rb_tree(const Compare& comp = Compare( ))
    : node_count(0), key_compare(comp)
  {
    init( );
  }

  ~rb_tree( )
  {
    clear( );
    put_node(header);
  }

  iterator begin( ) { return leftmost( ); }
  iterator end( ) { return header( ); }
  bool empty( ) const { return node_count == 0; }
  size_type size( ) const { return node_count; }
  size_type max_size( ) const { return size_type(-1); }

  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

public:
  pair<iterator, bool> insert_unique(const value_type& x);
  iterator insert_equal(const value_type& x);

private:
  iterator insert(base_ptr x, base_ptr y, const value_type& v);
  link_type copy(link_type x, link_type p);
  void erase(link_type x);

  void init( )
  {
    header = get_node( );
    color(header) = S_rb_tree_red;
    root( ) = nullptr;
    leftmost( ) = header;
    rightmose( ) = header;
  }

protected:
  link_type get_node( ) { return rb_tree_node_allocator::allocate( ); }
  void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

  link_type create_node(const value_type& x)
  {
    link_type tmp = get_node( );

    try {
      construct(&tmp->value_field_, x);
    } catch (...) {
      put_node(tmp);
      throw;
    }
    return tmp;
  }


  link_type clone_node(link_type x)
  {
    link_type tmp = create_node(x->value_field_);
    tmp->color_ = x->color_;
    tmp->left_ = nullptr;
    tmp->right_ = nullptr;
    return tmp;
  }

  void destory_node(link_type p)
  {
    destory(&p->value_field_);
    put_node(p);
  }


protected:
  link_type& root( ) const { return (link_type&)header->parent_; }
  link_type& leftmost( ) const { return (link_type&)header->left_; }
  link_type& rightmose( ) const { return (link_type&)header->right_; }

  static link_type& left(link_type x) { return (link_type&)(x->left_); }
  static link_type& right(link_type x) { return (link_type&)(x->right_); }
  static link_type& parent(link_type x) { return (link_type&)(x->parent_); }
  static reference value(link_type x) { return (link_type&)(x->value_field_); }
  static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
  static color_type& color(link_type x) { return (color_type&)(x->color_); }

  static link_type& left(base_ptr x) { return (link_type&)(x->left_); }
  static link_type& right(base_ptr x) { return (link_type&)(x->right_); }
  static link_type& parent(base_ptr x) { return (link_type&)(x->parent_); }
  static reference value(base_ptr x) { return ((link_type)x)->value_field_; }
  static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
  static color_type& color(base_ptr x) { return (color_type&)(((link_type)x)->color_); }

  static link_type maximum(link_type x) { return (link_type)RbTreeNodeBase::Maximun(x); }
  static link_type minimum(link_type x) { return (link_type)RbTreeNodeBase::Minimun(x); }

protected:
  size_type node_count;
  link_type header;
  Compare key_compare;
};


template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree::insert_equal(const value_type& x)
{
  link_type y = header;
  link_type x = root( );

  while (x != nullptr) {
    y = x;
    y = key_compare(KeyOfValue( )(v), key(x)) ? left(x), right(x);
  }
  return insert(x, y, v);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree::insert_unique(const value_type& x)
{

}



} // namespace ministl

#endif // MINISTL_RB_TREE_H