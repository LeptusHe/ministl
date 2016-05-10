#ifndef MINISTL_NUMERIC_H
#define MINISTL_NUMERIC_H

#include "iterator_base.h"

namespace ministl {

template <typename InputeIterator, typename T>
T accumulate(InputeIterator first, InputeIterator last, T init)
{
  for (; first != last; ++first)
    init = init + *first;
  return init;
}

template <typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op)
{
  for (; first != last; ++first)
    init = binary_op(init, *first);
  return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result)
{
  typedef iterator_traits<InputIterator>::value_type T;

  if (first == last)
    return result;

  *result = *first;
  T value = *first;
  while (++first != last) {
    T tmp = *first;
    *++result = tmp - value;
    value = tmp;
  }
  return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, BinaryOperation binary_op)
{
  typedef typename iterator_traits<InputIterator>::value_type T;

  if (first == last)
    return result;

  *result = *first;
  T value = *first;
  while (++first != last) {
    T tmp = *first;
    *++result = binary_op(tmp, value);
    value = tmp;
  }
  return ++result;
}


template <typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init)
{
  for (; first1 != last1; ++first1, ++first2)
    init = init + (*first1 * *first2);
  return init;
}

template <typename InputIterator1, typename InputIterator2, typename T,
  typename BinaryOperation1, typename BinaryOperation2>
  T inner_product(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, T init,
                  BinaryOperation1 binary_op1, BinaryOperation2 binary_op2)
{
  for (; first1 != last1; ++first1, ++first2)
    init = binary_op1(init, binary_op2(*first1, *first2));
  return init;
}


template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result)
{
  typedef typename iterator_traits<OutputIterator>::value_type T;

  if (first == last)  return result;

  *result = *first;
  T value = *first;
  while (++first != last) {
    value = value + *first;
    *++result = value;
  }
  return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op)
{
  typedef typename iterator_traits<OutputIterator>::value_type T;

  if (first == last)  return result;

  *result = *first;
  T value = *first;
  while (++first != last) {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}


template <typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
  while (first != last)
    *first++ = value++;
}

} // namespace ministl

#endif // MINISTL_NUMERIC_H