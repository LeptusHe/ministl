#ifndef MINISTL_FUNCTOOLS_H
#define MINISTL_FUNCTOOLS_H

namespace ministl {

template <typename ForwardIterator, typename Operation>
inline void map(ForwardIterator first, ForwardIterator last, Operation op)
{
  for (; first != last; ++first) {
    *first = op(*first);
  }
}

} // namespace ministl

#endif // MINISTL_FUNCTOOLS_H