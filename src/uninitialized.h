#ifndef MINISTL_UNINITIALIZED_H
#define MINISTL_UNINITIALIZED_H

#include <string.h>
#include <stdlib.h>
#include "algorithm.h"
#include "construct.h"

namespace ministl {

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
{
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

template <typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1 *)
{
    typedef typename type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, x, is_POD( ));
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x,
                                                    true_type)
{
    return fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x,
                                                    false_type)
{
    ForwardIterator cur = first;
    for (; n > 0; --n, ++cur)
        construct(&*cur, x);
    return cur;
}


template <typename InputItertor, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputItertor first, InputItertor last,
                                            ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

template <typename InputItertor, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputItertor first, InputItertor last,
                                            ForwardIterator result, T*)
{
    typedef typename type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD( ));
}

template <typename InputItertor, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputItertor first, InputItertor last,
                                                ForwardIterator result, true_type)
{
    return copy(first, last, result);
}

template <typename InputItertor, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputItertor first, InputItertor last,
                                                ForwardIterator result, false_type)
{
    ForwardIterator cur = result;

    for (; first != last; ++first, ++cur)
        construct(&*cur, *first);
    return cur;
}

inline char* uninitialized_copy(const char *first, const char *last, char *result)
{
    memmove(result, first, last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
    memmove(result, first, sizeof(wchar_t)* (last - first));
    return result + (last - first);
}


template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                const T &x)
{
    return __uninitialized_fill(first, last, x, value_type(first));
}

template <typename ForwardIterator, typename T, typename T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, T1 *)
{
    typedef typename type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_aux(first, last, x, is_POD( ));
}

template <typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                        const T &x, true_type)
{
    fill(first, last, x);
}

template <typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                        const T &x, false_type)
{
    ForwardIterator cur = first;

    for (; cur != last; ++cur)
        construct(&*cur, x);
}

}  // namespace ministl

#endif // MINISTL_UNINITIALIZED_H