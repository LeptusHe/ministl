#ifndef MINISTL_ITERATOR_BASE_H
#define MINISTL_ITERATOR_BASE_H

#include <cstddef>

namespace ministl {

struct input_iterator_tag  { };
struct output_iterator_tag { };
struct forward_iterator_tag: public input_iterator_tag { };
struct bidirectional_iterator_tag: public forward_iterator_tag { };
struct random_access_iterator_tag: public bidirectional_iterator_tag { };

template <typename Tp, typename Distance> 
struct input_iterator {
    typedef input_iterator_tag iterator_category;
    typedef Tp                 value_type;
    typedef Distance           different_type;
    typedef Tp*                pointer;
    typedef Tp&                reference;
};

struct output_iterator {
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                different_type;
    typedef void                pointer;
    typedef void                reference;
};

template <typename Tp, typename Distance> 
struct forward_iterator {
    typedef forward_iterator_tag iterator_category;
    typedef Tp                 value_type;
    typedef Distance           different_type;
    typedef Tp*                pointer;
    typedef Tp&                reference;
};

template <typename Tp, typename Distance> 
struct bidirectional_iterator {
    typedef bidirectional_iterator iterator_category;
    typedef Tp                     value_type;
    typedef Distance               different_type;
    typedef Tp*                    pointer;
    typedef Tp&                    reference;
};

template <typename Tp, typename Distance> 
struct random_access_iterator {
    typedef random_access_iterator iterator_category;
    typedef Tp                     value_type;
    typedef Distance               different_type;
    typedef Tp*                    pointer;
    typedef Tp&                    reference;
};


template <typename Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template <class Tp>
struct iterator_traits<Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef Tp                         value_type;
    typedef ptrdiff_t                  difference_type;
    typedef Tp*                        pointer;
    typedef Tp&                        reference;
};

template <class Tp>
struct iterator_traits<const Tp*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef Tp                          value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const Tp*                   pointer;
    typedef const Tp&                   reference;
};


// functions used to get the associated type of iterators
template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category 
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category( );
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::differece_type* 
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference*>(0);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// functions distance used to calculate the distance of two iterator
template <typename InputIteator>
inline typename iterator_traits<InputIteator>::difference_type
distance(InputIteator first, InputIteator last)
{
    typedef typename iterator_traits<InputIteator>::iterator_category category;
    return __distance(first, last, category());
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    iterator_traits<InputIterator>::difference_type n = 0;

    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{ return last - first; }


// function advance used to make iterator advance n(n maybe negitive)
template <typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance n)
{
    __advance(i, n, iterator_category(i));
}

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance n,
                        input_iterator_tag)
{
    while (n--) ++i;
}

template <typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator& i, Distance n,
                        bidirectional_iterator_tag)
{
    if (n >= 0)
    while (n--) ++i;
    else
    while (n++) --i;
}

template <typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator& i, Distance n,
                        random_access_iterator_tag)
{
    i += n;
}

} // namespace ministl

#endif // MINISTL_ITERATOR_BASE_H