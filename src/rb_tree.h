#ifndef MINISTL_RB_TREE_H
#define MINISTL_RB_TREE_H

#include "iterator_base.h"

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



} // namespace ministl

#endif // MINISTL_RB_TREE_H