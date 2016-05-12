#ifndef MINISTL_ALGORITHM_H
#define MINISTL_ALGORITHM_H

#include <cstring>
#include "pair.h"
#include "type_traits.h"
#include "iterator_base.h"

namespace ministl {

template <typename T>
inline const T& max(const T &a, const T &b)
{
  return a < b ? b : a;
}

template <typename T, typename Compare>
inline const T& max(const T &a, const T &b, Compare comp)
{
  return comp(a, b) ? b : a;
}

// -----------------------------------------------------------------------------
// find algorithms
//------------------------------------------------------------------------------
// names of algorithms: find, find_if, count, count_if
//------------------------------------------------------------------------------
template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T &x)
{
  while (first != last && *first != x)
    ++first;
  return first;
}

template <typename InputIterator, typename Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
{
  while (first != last && !pred(*first))
    ++first;
  return first;
}

template <typename InputIterator, typename Predicate>
InputIterator find_if_not(InputIterator first, InputIterator last,
                          Predicate pred)
{
  while (first != last && pred(*first))
    ++first;
  return first;
}

template <typename InputIterator, typename T>
typename iterator_traits<T>::difference_type
count(InputIterator first, InputIterator last, const T& value)
{
  typename iterator_traits<T>::difference_type n = 0;

  for (; first != last; ++first) {
    if (*first == value)
      ++n;
  }
  return n;
}

template <typename InputIterator, typename Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;

  for (; first != last; ++first) {
    if (pred(*first))
      ++n;
  }
  return n;
}

//------------------------------------------------------------------------------
// algorithms used to find elements which are adjacent
//------------------------------------------------------------------------------
// names of algorithms: adjacent_find, search_n
//------------------------------------------------------------------------------
template <typename ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
{
  if (first == last)
    return last;

  ForwardIterator next = first;
  while (++next != last) {
    if (*first == *next)
      return first;
    first = next;
  }
  return last;
}

template <typename ForwardIterator, typename BinaryPredicate>
ForwardIterator adjacent_if(ForwardIterator first, ForwardIterator last,
                            BinaryPredicate binary_pred)
{
  if (first == last)
    return last;

  ForwardIterator next = first;
  while (++next != last) {
    if (binary_pred(*first, *next))
      return first;
    first = next;
  }
  return last;
}


template <typename ForwardIterator, typename Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value)
{
  if (count <= 0)
    return first;
  else {
    first = find(first, last, value);
    while (first != last) {
      Integer n = count - 1;
      ForwardIterator i = first;
      ++i;
      while (i != last && *i = value && n != 0) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else
        first = find(i, last, value);
    }
    return last;
  }
}


template <typename ForwardIterator, typename Integer,
  typename T, typename BinaryPredicate>
  ForwardIterator search_n(ForwardIterator first,
                           ForwardIterator last,
                           Integer count, const T& value,
                           BinaryPredicate binary_pred)
{
  if (count <= 0)
    return first;
  else {
    while (first != last) {
      if (binary_pred(*first, value))
        break;
      ++first;
    }

    while (first != last) {
      Integer n = count - 1;
      ForwardIterator i = first;
      ++i;

      while (i != last && n != 0 && binary_pred(*i, value)) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else {
        while (i != last) {
          if (binary_pred(*i, value))
            break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
}


//------------------------------------------------------------------------------
// algorithms used to find subsequence
//------------------------------------------------------------------------------
// names of algorithms: search, find_first_of, find_end
//------------------------------------------------------------------------------
template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2)
{
  return __search(first1, last1, first2, last2, distance_type(first1),
                  distance_type(first2));
}


template <typename ForwardIterator1, typename ForwardIterator2,
  typename Distance1, typename Distance2>
  ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator2 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            Distance1 *, Distance2 *)
{
  Distance1 d1 = 0;
  distance(first1, last1, d1);

  Distance2 d2 = 0;
  distance(first2, last2, d2);

  if (d1 < d2) return last1;

  ForwardIterator1 current1 = first1;
  ForwardIterator2 current2 = first2;

  while (current2 != last2) {
    if (*current1 == *current2) {
      ++current1;
      ++current2;
    } else {
      if (d1 == d2)
        return last1;
      else {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  }
  return first1;
}


template <typename InputIterator, typename ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2)
{
  for (; first1 != last1; ++first1) {
    for (auto iter = first2; iter != last2; ++first2) {
      if (*first1 == *iter)
        return first1;
    }
  }
  return last1;
}


template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            BinaryPredicate comp)
{
  for (; first1 != last1; ++first1) {
    for (auto iter = first2; iter != last2; ++first2) {
      if (comp(*first1, *iter))
        return first1;
    }
  }
  return last1;
}


template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2)
{
  using category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
  using category2 = typename iterator_traits<ForwardIterator2>::iterator_category;

  return __find_end(first1, last1, first2, last2, category1( ), category2( ));
}

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag)
{
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (true) {
      ForwardIterator1 new_result = search(first1, last1, first2, last2);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

template <typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator1
__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
           BidirectionalIterator2 first2, BidirectionalIterator2 last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  using reviter1 = reverse_iterator<BidirectionalIterator1>;
  using reviter2 = reverse_iterator<BidirectionalIterator2>;

  reviter1 rlast1(first1);
  reviter2 rlast2(first2);

  reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base( );
    advance(result, -distance(first2, last2));
    return result;
  }
}

// algorithm equal
template <typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
  for (; first1 != last1; ++first1, ++first2) {
    if (*first1 != *first2)
      return false;
  }
  return true;
}

template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1,
           InputIterator2 first2, BinaryPredicate binary_pred)
{
  for (; first1 != last1; ++first1, ++first2) {
    if (!binary_pred(first1, first2))
      return false;
  }
  return true;
}

// algorithm iter_swap
template <typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{

  typename iterator_traits<ForwardIterator1>::value_type T tmp = *a;
  *a = *b;
  *b = tmp;
}

// algorithm swap
template <typename T>
inline void swap(T &a, T &b)
{
  T tmp = a;
  a = b;
  b = tmp;
}


// algorithm lexicographical_compare
template <typename InputIterator1, typename InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                    InputIterator2 first2, InputIterator2 last2)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first2)
      return false;
  }
  return first1 == last1 && first2 == last2;
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
inline bool lexicographical_compare(InputIterator1 first1,
                                    InputIterator1 last1,
                                    InputIterator2 first2,
                                    InputIterator2 last2,
                                    Compare comp)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first2))
      return false;
  }
  return first1 == last1 && first2 == last2;
}

template <typename T>
inline bool lexicographical_compare(const unsigned char *first1,
                                    const unsigned char *last1,
                                    const unsigned char *first2,
                                    const unsigned char *last2)
{
  const size_t len_1 = last1 - first1;
  const size_t len_2 = last2 - first2;

  const int result = memcmp(first1, first2, min(len_1, len_2));
  return result != 0 ? result < 0 : len_1 < len_2;
}

template <typename T>
inline bool lexicographical_compare(const char *first1, const char *last1,
                                    const char *first2, const char *last2)
{
  const size_t len_1 = last1 - first1;
  const size_t len_2 = last2 - first2;

  const int result = memcmp(first1, first2, min(len_1, len_2));
  return result != 0 ? result < 0 : len_1 < len_2;
}


// algorithm mismatch
template <typename InputIterator1, typename InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2)
{
  while (first1 != last1 && *first1 != *first2) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2,
                                              BinaryPredicate binary_pred)
{
  while (first1 != last1 && binary_pred(*first1, *first2)) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}


// copy algorithm
inline char* copy(const char *first, const char *last, char *result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
  memmove(result, first, sizeof(wchar_t)* (last - first));
  return result + (last - first);
}

template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result)
{
  return copy_dispatch<InputIterator, OutputIterator>( )(first, last, result);
}

// copy_dispatch
template <typename InputIterator, typename OutputIterator>
struct copy_dispatch {
  OutputIterator operator() (InputIterator first,
                             InputIterator last,
                             OutputIterator result)
  {
    return copy(first, last, result, iterator_category(first));
  }
};

template <typename T>
struct copy_dispatch<T*, T*> {
  T* operator() (T* first, T* last, T* result)
  {
    typedef typename type_traits<T>::has_trivial_assignment_operator t;
    return copy_t(first, last, result, t( ));
  }
};

template <typename T>
struct copy_dispatch<const T*, T*> {
  T* operator() (const T* first, const T* last, T* result)
  {
    typedef typename type_traits<T>::has_trivial_assignment_operator t;
    return copy_t(first, last, result, t( ));
  }
};

// copy_t functions
template <typename T>
inline T* copy_t(const T *first, const T *last, T *result, true_type)
{
  memmove(result, first, sizeof(T)* (last - first));
  return result + (last - first);
}

template <typename T>
inline T* copy_t(const T *first, const T *last, T *result, false_type)
{
  return copy_d(first, last, result, (ptrdiff_t*)0);
}

// copy functions
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result, input_iterator_tag)
{
  for (; first != last; ++first, ++result)
    *result = *first;
  return result;
}

template <typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator copy(RandomAccessIterator first, RandomAccessIterator last,
                           OutputIterator result, random_access_iterator_tag)
{
  return copy_d(first, last, result, distance_type(first));
}

template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator copy_d(RandomAccessIterator first, RandomAccessIterator last,
                             OutputIterator result, Distance *)
{
  for (Distance n = last - first; n > 0; --n, ++result, ++first)
    *result = *first;
  return result;
}

// fill and fill_n
template <typename ForwardIter, typename Tp>
void fill(ForwardIter first, ForwardIter last, const Tp &value)
{
  for (; first != last; ++first)
    *first = value;
}

template <typename OutputIter, typename Size, typename Tp>
OutputIter fill_n(OutputIter first, Size n, const Tp& value)
{
  for (; n > 0; --n, ++first)
    *first = value;
  return first;
}

// Specialization: for one-byte types we can use memset.
inline void fill(unsigned char* first, unsigned char* last,
                 const unsigned char &c)
{
  unsigned char tmp = c;
  memset(first, tmp, last - first);
}

inline void fill(signed char* first, signed char* last,
                 const signed char& c)
{
  signed char tmp = c;
  memset(first, static_cast<unsigned char>(tmp), last - first);
}

inline void fill(char* first, char* last, const char& c)
{
  char tmp = c;
  memset(first, static_cast<unsigned char>(tmp), last - first);
}

// algorithm: copy_backward
template <typename BidirectionalIter1, typename BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first,
                                        BidirectionalIter1 last,
                                        BidirectionalIter2 result)
{
  typedef typename type_traits<typename iterator_traits<BidirectionalIter2>::value_type>
    ::has_trivial_assignment_operator Trivial;

  return __copy_backward_dispatch<BidirectionalIter1, BidirectionalIter2, Trivial>::copy(first, last, result);
}

// copy_dispatch
template <typename BidirectionalIter1, typename BidirectionalIter2, typename BoolType>
struct __copy_backward_dispatch {
  typedef typename iterator_traits<BidirectionalIter1>::iterator_category category;
  typedef typename iterator_traits<BidirectionalIter1>::difference_type   Distance;

  static BidirectionalIter2 copy(BidirectionalIter1 first,
                                 BidirectionalIter1 last,
                                 BidirectionalIter2 result)
  {
    return __copy_backward(first, last, result, category( ), (Distance*)0);
  }
};

template <typename Tp>
struct __copy_backward_dispatch<Tp*, Tp*, true_type> {
  static Tp* copy(const Tp* first, const Tp* last, Tp* result)
  {
    const ptrdiff_t Num = last - first;
    memmove(result - Num, first, sizeof(Tp)* Num);
    return result - Num;
  }
};

template <typename Tp>
struct __copy_backward_dispatch<const Tp*, Tp*, true_type> {
  static Tp* copy(const Tp* first, const Tp* last, Tp* result)
  {
    return  __copy_backward_dispatch<Tp*, Tp*, true_type>::copy(first, last, result);
  }
};

// _copybackward
template <typename BidirectionalIter1, typename BidirectionalIter2, typename Distance>
inline BidirectionalIter2 _copybackward(BidirectionalIter1 first,
                                        BidirectionalIter1 last,
                                        BidirectionalIter2 result,
                                        bidirectional_iterator_tag, Distance*)
{
  while (first != last)
    *--result = *--last;
  return result;
}

template <typename RandomAccessIter, typename BidirectionalIter, typename Distance>
inline BidirectionalIter __copy_backward(RandomAccessIter first,
                                         RandomAccessIter last,
                                         BidirectionalIter result,
                                         random_access_iterator_tag,
                                         Distance*)
{
  for (Distance n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}


template <typename ForwardIterator, typename Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen)
{
  for (; first != last; ++first)
    *first = gen( );
}

} // namespace ministl

#endif // MINISTL_ALGORITHM_H