#ifndef HASHTABLE_H
#define HASHTABLE_H

namespace ministl {

template <typename Value>
struct hashtable_node {
  HashtableNode* next;
  Value val;
};

template <typename Value, typename Key, typename HashFunc,
          typename ExtractKey, typename EqualKey, typename Alloc>
struct hashtable_iterator {
  using hashtable = hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using iterator = hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using const_iterator = hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using node = hashtable_node<Value>;
};





} // namespace ministl

#endif