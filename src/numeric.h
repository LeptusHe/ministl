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
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
    for (; first != last; ++first)
        init = binary_op(init, *first);
    return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
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
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op)
{
    typedef iterator_traits<InputIterator>::value_type T;

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

} // namespace tinyc

#endif // MINISTL_NUMERIC_H