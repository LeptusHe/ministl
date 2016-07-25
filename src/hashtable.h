#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "alloc.h"
#include <vector>
#include <algorithm>

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
  
  using iterator_category = forward_iterator_tag;
  using value_type = Value;
  using size_type = size_t;
  using reference = Value&;
  using pointer = Value*;

public:
  node* cur;
  hashtable* ht;

public:
  hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) { }
  hashtable_iterator( ) : cur(nullptr), ht(nullptr) { }
  reference operator*( ) const { return cur->val; }
  point operator->( ) const { return &(operator*( )); }
  iterator operator++( );
  iterator operator++(int);
  bool operator==(const iterator& it) const { return cur == it->cur; }
  bool operator!=(const iterator& it) const { return cur != it->cur; }
};

template <typename V, typename K, typename HF, typename ExK, typename EqK, typename A>
hashtable_iterator<V, K, HF, ExK, EqK, A>&
hashtable_iterator::operator ++( )
{
  const node* old = cur;
  cur = cur->next;
  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets.size( ))
      cur = ht->buckets[bucket];
  }
  return *this;
}

template <typename Value, typename Key, typename HashFunc, typename ExtratKey, typename EqualKey, typename Alloc>
hashtable_iterator<Value, Key, HashFunc, ExtratKey, EqualKey, Alloc>
hashtable_iterator<Value, Key, HashFunc, ExtratKey, EqualKey, Alloc>::operator ++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}


template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Alloc>
struct hashtable_const_iterator {
  using hashtable = hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using iterator = hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using const_iterator = hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using node = hashtable_node<Value>;
  using iterator_category = forward_iterator_tag;
  using value_type = Value;
  using difference_type = ptrdiff_t;
  using size_type = size_t;
  using reference = const Value&;
  using pointer = const Value*;

public:
  const node* cur_;
  const hashtable* ht_;

public:
  hashtable_const_iterator(const node* n, const hashtable* tab)
    : cur_(n), ht_(tab)
  { }

  hashtable_const_iterator( ) { }
  hashtable_const_iterator(const iterator& it)
    : cur_(it.cur_), ht_(it.ht_)
  { }

  reference operator*( ) const { return cur_->val; }
  pointer operator->( ) const { return &(operator*( )); }
  bool operator==(const const_iterator& it) const
  {
    return cur_ == it.cur_;
  }

  bool operator!=(const const_iterator& it) const
  {
    return cur_ != it.cur_;
  }
};

static const int stl_num_primes = 28;
static const unsigned long stl_prime_list[stl_num_primes] = {
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long stl_next_prime(unsigned long n)
{
  const unsigned long* first = stl_prime_list;
  const unsigned long* last = stl_prime_list + stl_num_primes;
  const unsigned long* pos = std::lower_bound(first, last, n);
  return pos == last ? *(last - 1) : *pos;
}



template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Alloc>
class hashtable;

template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Alloc>
bool operator==(const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& lhs,
                const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& rhs);


template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
class hashtable {
public:
  using key_type = Key;
  using value_type = Value;
  using hasher = HashFunc;
  using key_equal = EqualKey;

  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
  using const_iterator = hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;

public:
  friend struct hashtable_iterator;
  friend struct hashtable_const_iterator;

public:
  hasher hash_funct( ) const { return hash_; }
  key_equal key_eq( ) const { return equals_; }

private:
  using node = hashtable_node<Value>;

public:
  using node_allocator = simple_alloc<node, Alloc>;

private:
  node* get_node( )
  {
    return node_allocator::allocate(1);
  }

  node* put_node(node* p)
  {
    node_allocator::deallocate(p, 1);
  }

public:
  hashtable(size_type n, const HashFunc& hf, const EqualKey& eql)
    : hash_(hf), equals_(eql), get_key_(ExtractKey( )), num_elements_(0)
  {
    initialize_buckets(n);
  }

  hashtable& operator==(const hashtable& ht)
  {
    if (&ht != this) {
      clear( );
      hash_ = ht.hash_;
      equals_ = ht.equals_;
      get_key_ = ht.get_key_;
      copy_from(ht);
    }
    return *this;
  }

  ~hashtable( ) { clear( ); }

  size_type size( ) const { return num_elements_; }
  size_type max_size( ) const { return size_type(-1); }
  bool empty( ) const { return size( ) == 0; }

  void swap(hashtable& ht)
  {
    std::swap(hash_, ht.hash_);
    std::swap(equals_, ht.equals_);
    std::swap(get_key_, ht.get_key_);
    buckets_.swap(ht.buckets_);
    std::swap(num_elements_, ht.num_elements_);
  }

  iterator begin( )
  {
    for (size_type n = 0; n < buckets_.size( ); ++n) {
      if (buckets_[n]) {
        return iterator(buckets_[n], this);
      }
    }
    return end( );
  }

  iterator end( )
  {
    return iterator(0, this);
  }

  size_type bucket_count( ) const { return buckets_.size( ); }

  size_type max_bucket_count( ) const
  {
    return stl_prime_list[stl_num_primes - 1];
  }




  pair<iterator, bool> insert_unique(const value_type& obj)
  {
    resize(num_elements_ + 1);
    return insert_unique_noresize(obj);
  }

  void resize(size_type num_elements_hint)
  {
    const size_type old_n = buckets_.size( );
    if (num_elements_hint > old_n) {
      const size_type n = next_size(num_elements_hint);
      if (n > old_n) {
        std::vector<node*, Alloc> tmp(n, (node*)0);
        try {
          for (size_type bucket = 0; bucket < old_n; ++bucket) {
            node* first = buckets_[bucket];
            while (first) {
              size_type new_bucket = bkt_num(first->val);
              buckets_[bucket] = first->next;
              first->next = tmp[new_bucket];
              tmp[new_bucket] = first;
              first = buckets_[bucket];
            }
          }
          buckets_.swap(tmp);
        }
      }
    }
  }

  pair<iterator, bool> insert_unique_noresize(const value_type& obj)
  {
    const size_type n = bkt_num(obj);
    node* first = buckets_[n];

    for (node* cur = first; cur; cur = cur->next) {
      if (equals_(get_key_(cur->val), get_key_(obj)))
        return pair<iterator, bool>(iterator(cur, this), false);
    }

    node* tmp = new_node(obj);
    tmp->next = first;
    buckets_[n] = tmp;
    ++num_elements_;
    return pair<iterator, bool>(iterator(tmp, this), true);
  }

  iterator insert_equal(const value_type& obj)
  {
    resize(num_elements_ + 1);
    return insert_equal_noresize(obj);
  }

  iterator insert_equal_noresize(const value_type& obj)
  {
    const size_type n = bkt_num(obj);
    node* first = buckets_[n];

    for (node* cur = first; cur; cur = cur->next) {
      if (equals_(get_key_(cur->val), get_key_(obj))) {
        node* tmp = new_node(obj);
        tmp->next = cur->next;
        cur->next = tmp;
        ++num_elements_;
        return iterator(tmp, this);
      }
    }

    node* tmp = new_node(obj);
    tmp->next = first;
    buckets_[n] = tmp;
    ++num_elements_;
    return iterator(tmp, this);
  }

  iterator find(const key_type& key)
  {
    size_type n = bkt_num_key(key);
    node* first;
    for (first = buckets_[n];
    first && !equals_(get_key_(first->val), key);
      first = first->next) {

    }
    return const_iterator(first, this);
  }



  void clear( )
  {
    for (size_type i = 0; i < buckets_.size( ); ++i) {
      node* cur = buckets_[i];
      while (cur != 0) {
        node* next = cur->next;
        delete_node(cur);
        cur = next;
      }
      buckets_[i] = 0;
    }
    num_elements_ = 0;
  }

  void copy_from(const hashtable& ht)
  {
    buckets_.clear( );
    buckets_.resize(ht.buckets_.size( ));
    buckets_.insert(buckets_.end( ), ht.buckets_.size( ); (node*)0);
    try {
      for (size_type i = 0; i < ht.buckets_.size( ); ++i) {
        if (const node* cur = ht.buckets_[i]) {
          node* copy = new_node(cur->val);
          buckets_[i] = copy;

          for (node* next = cur->next; next; cur = next, next = cur->next) {
            copy->next = new_node(next->val);
            copy = copy->next;
          }
        }
        num_elements_ = ht.num_elements_;
      }
    } catch (...) {
      clear( );
      throw;
    }
  }

private:
  void initialize_buckets(size_type n)
  {
    const size_type n_buckets = next_size(n);
    buckets_.reserve(n_buckets);
    buckets_.insert(buckets_.end( ), n_buckets, (node*)0);
    num_elements_ = 0;
  }


  size_type next_size(size_type n) const { return stl_num_primes(n); }

  size_type bkt_num(const value_type& obj, size_t n) const
  {
    return bkt_num_key(get_key_(obj), n);
  }

  size_type bkt_num(const value_type& obj) const
  {
    return bkt_num_key(get_key_(obj));
  }

  size_type bkt_num_key(const key_type& key) const
  {
    return bkt_num_key(key, buckets_.size( ));
  }

  size_type bkt_num_key(const key_type& key, size_t n) const
  {
    return hash(key) % n;
  }

  node* new_node(const value_type& obj)
  {
    node* n = get_node( );
    n->next = 0;
    try {
      construct(&n->val, obj);
      return n;
    } catch (...) {
      put_node(n);
      throw;
    }
  }

  void delete_node(node* n)
  {
    destory(&n->val);
    put_node(n);
  }

private:
  hasher hash_;
  key_equal equals_;
  ExtractKey get_key_;

  std::vector<node*, Alloc> buckets_;
  size_type num_elements_;
};

} // namespace ministl

#endif