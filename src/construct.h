#ifndef MINISTL_CONSTRUCT_H
#define MINISTL_CONSTRUCT_H

#include <new.h>
#include "type_traits.h"
#include "iterator_base.h"

namespace ministl {

template <class T1, class T2>
inline void construct(T1 *p, const T2 &value) { new(p)T1(value); }

template <typename T>
inline void destory(T* pointer) { pointer->~T( ); }

template <typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) { __destory(first, last, value_type(first)); }

template <class ForwardIterator, class T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destory_aux(first, last, trivial_destructor( ));
}

template <typename ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first < last; ++first)
        destory(&*first);
}

template <typename ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) { }
inline void destory(char *, char *) { }
inline void destory(wchar_t *, wchar_t *) { }

} // namespace ministl

#endif  // MINISTL_CONSTRUCT_H