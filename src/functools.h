#ifndef MINISTL_FUNCTOOLS_H
#define MINISTL_FUNCTOOLS_H

namespace ministl {

//------------------------------------------------------------------------------
// algorithms for set
//------------------------------------------------------------------------------
// names of algorithm: all_of, any_of, none_of
//------------------------------------------------------------------------------
template <typename InputIterator, typename Predicate>
bool all_of(InputIterator first, InputIterator last, Predicate pred)
{
  bool flag = true;

  for (; first != last && flag; ++first) {
    if (!pred(*first))
      flag = false;
  }
  return flag;
}

template <typename InputIterator, typename Predicate>
bool any_of(InputIterator first, InputIterator last, Predicate pred)
{
  bool flag = false;

  for (; first != last && !flag; ++first) {
    if (pred(*first))
      flag = true;
  }
  return flag;
}

template <typename InputIterator, typename Predicate>
bool none_of(InputIterator first, InputIterator last, Predicate pred)
{
  bool flag = true;

  for (; first != last && flag; ++first) {
    if (pred(*first))
      flag = false;
  }
  return flag;
}

//------------------------------------------------------------------------------
// name of algorithms: map
//------------------------------------------------------------------------------
template <typename ForwardIterator, typename Operation>
inline void map(ForwardIterator first, ForwardIterator last, Operation op)
{
  for (; first != last; ++first) {
    *first = op(*first);
  }
}

//------------------------------------------------------------------------------
// name of algorithm: for_each
//------------------------------------------------------------------------------
template <typename InputIterator, typename Operation>
inline void for_each(InputIterator first, InputIterator last, Operation op)
{
  for (; first != last; ++first)
    op(*first);
}

} // namespace ministl

#endif // MINISTL_FUNCTOOLS_H