#ifndef MINISTL_ITERATOR_H
#define MINISTL_ITERATOR_H

#include "iterator_base.h"

namespace ministl {

template <typename Container>
class back_insert_iterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                pointer;
  typedef void                reference;
  typedef void                difference_type;

protected:
  Container* container;

public:
  explicit back_insert_iterator(const Container& container) : container(&container) { }
  back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
  {
    container->push_back(value);
    return *this;
  }

  back_insert_iterator<Container>& operator*( ) { return *this; }
  back_insert_iterator<Container>& operator++( ) { return *this; }
  back_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container>
inline back_insert_iterator<Container> back_inserter(Container& x)
{
  return back_insert_iterator<Container>(x);
}


template <typename Container>
class front_insert_iterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                reference;
  typedef void                difference_type;

protected:
  Container* container;
public:
  explicit front_insert_iterator(const Container& x) : container(&x) { }
  front_insert_iterator& operator=(const typename Container::value_type& value)
  {
    container->push_front( );
    return *this;
  }

  front_insert_iterator& operator*( ) { return *this; }
  front_insert_iterator& operator++( ) { return *this; }
  front_insert_iterator& operator++(int) { return *this; }
};

template <typename Container>
back_insert_iterator<Container> front_inserter(Container& x)
{
  return back_insert_iterator<Container>(x);
}


template <typename Container>
class insert_iterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                pointer;
  typedef void                reference;
  typedef void                difference_type;

protected:
  Container* container;
  typename Container::iterator iter;
public:
  insert_iterator(Container& container, typename Container::iterator& i)
    : container(&x), iter(i)
  { }

  insert_iterator<Container>& operator=(const typename Container::value_type& value)
  {
    iter = container->insert(iter, value);
    ++iter;
    return *this;
  }

  insert_iterator<Container>& operator*( ) { return *this; }
  insert_iterator<Container>& operator++( ) { return *this; }
  insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container, typename Iterator>
back_insert_iterator<Container> inserter(Container& x, Iterator i)
{
  typedef typename Container::iterator iter;
  return insert_iterator<Container>(x, iter(i));
}


template <typename Iterator>
class reverse_iterator {
public:
  typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename iterator_traits<Iterator>::value_type        value_type;
  typedef typename iterator_traits<Iterator>::pointer           pointer;
  typedef typename iterator_traits<Iterator>::reference         reference;
  typedef typename iterator_traits<Iterator>::difference_type   differenec_type;

  typedef Iterator iterator_type;
  typedef reverse_iterator<Iterator> Self;
public:
  reverse_iterator( ) { }
  explicit reverse_iterator(iterator_type x) : current(x) { }
  reverse_iterator(const Self& x) : current(x.current) { }

  iterator_type base( ) const { return current; }
  reference operator*( ) const
  {
    Iterator tmp = current;
    return *--tmp;
  }

  pointer operator->( ) const
  {
    return &(operator*( ));
  }

  Self& operator++( )
  {
    --current;
    return *this;
  }

  Self& operator++(int)
  {
    Self tmp = *this;
    --current;
    return tmp;
  }

  Self& operator--( )
  {
    ++current;
    return *this;
  }

  Self& operator--(int)
  {
    Self tmp = *this;
    ++current;
    return tmp;
  }

  Self operator+(differenec_type n) const
  {
    return Self(current - n);
  }

  Self& operator+=(differenec_type n)
  {
    current -= n;
    return *this;
  }

  Self operator-(differenec_type n)
  {
    return Self(current + n);
  }

  Self& operator-=(differenec_type n)
  {
    current += n;
    return *this;
  }

  reference operator[](differenec_type n) const
  {
    return *(*this + n);
  }

protected:
  Iterator current;
};


template <typename T, typename Distance = ptrdiff_t>
class istream_iterator {
  friend bool operator==<>(const istream_iterator<T>& x, const istream_iterator<T>& y);

public:
  typedef input_iterator_tag iterator_category;
  typedef T                  value_type;
  typedef const T*           pointer;
  typedef const T&           reference;
  typedef Distance           difference_type;

  istream_iterator( ) : stream(&cin), end_marker(false) { }
  istream_iterator(istream& s) : stream(s), end_marker(false) { read( ); }
  reference operator*( ) const { return value; }
  pointer operator->( ) const { return &(operator*( )); }
  istream_iterator<T, Distance>& operator++( )
  {
    read( );
    return *this;
  }

  istream_iterator<T, Distance>& operator++(int)
  {
    istream_iterator<T, Distance> tmp = *this;
    read( );
    return tmp;
  }

protected:
  istream* stream;
  T value;
  bool end_marker;

  void read( )
  {
    end_marker = (*stream) ? true : false;
    if (end_marker)
      *stream >> value;
    end_marker = (*stream) ? true : false;
  }

  bool equal(const istream_iterator& x) const
  {
    return (end_marker == end_marker) && (!end_marker || stream == x.stream);
  }

};

template <typename T>
friend bool operator==(const istream_iterator<T>& x, const istream_iterator<T>& y)
{
  x.equal(y);
}


template <typename T>
class ostream_iterator {
protected:
  ostream* stream;
  const char* string;

public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                pointer;
  typedef void                reference;
  typedef void                difference_type;

  ostream_iterator(ostream& os) : stream(os), string(nullptr) { }
  ostream_iterator(ostream& os, const char* c) : stream(&os), string(c) { }
  ostream_iterator<T>& operator=(const T& value)
  {
    *stream << value;
    if (string)
      *stream << string;
    return *this;
  }
  ostream_iterator<T>& operator*( ) { return *this; }
  ostream_iterator<T>& operator++( ) { return *this; }
  ostream_iterator<T>& operator++(int) { return *this; }
};

} // namespace ministl

#endif // MINISTL_ITERATOR_H