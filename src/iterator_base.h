#ifndef MINISTL_ITERATOR_BASE_H
#define MINISTL_ITERATOR_BASE_H

namespace ministl {

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

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
  typedef bidirectional_iterator_tag iterator_category;
  typedef Tp                         value_type;
  typedef Distance                   different_type;
  typedef Tp*                        pointer;
  typedef Tp&                        reference;
};

template <typename Tp, typename Distance>
struct random_access_iterator {
  typedef random_access_iterator_tag iterator_category;
  typedef Tp                         value_type;
  typedef Distance                   different_type;
  typedef Tp*                        pointer;
  typedef Tp&                        reference;
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


} // namespace ministl

#endif // MINISTL_ITERATOR_BASE_H