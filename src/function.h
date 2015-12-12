#ifndef MINISTL_FUNCTION_H
#define MINISTL_FUNCTION_H

namespace ministl {

template <typename Arg, typename Result>
class unary_function {
public:
  typedef Arg    argument_type;
  typedef Result result_type;
};

template <typename Arg1, typename Arg2, typename Result>
class binary_function {
public:
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};


// arithmetic functors
// the arithmetic functors contains plus, minus, multiples, divides, modulus factors.

template <typename Tp>
class plus : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) const { return x + y; }
};


template <typename Tp>
class minus : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) const { return x - y; }
};


template <typename Tp>
class multiplies : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) const { return x * y; }
};


template <typename Tp>
class divides : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) const { return x / y; }
};


template <typename Tp>
class modulus : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) const { return x % y; }
};


template <typename Tp>
class negate : public unary_function<Tp, Tp> {
public:
  Tp operator()(const Tp& x) const { return -x; }
};


template <typename Tp> inline Tp identity_element(plus<Tp>)
{
  return Tp(0);
}


template <typename Tp> inline Tp identity_element(multiplies<Tp>)
{
  return Tp(1);
}



// relational factors
// relational factors contains equal_to, not_equal_to, greater, greater_equal,
// less, less_equal factors.

template <typename Tp>
class equal_to : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x == y; }
};


template <typename Tp>
class not_equal_to : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x != y; }
};


template <typename Tp>
class greater : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x > y; }
};


template <typename Tp>
class less : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x < y; }
};


template <typename Tp>
class greater_equal : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x >= y; }
};


template <typename Tp>
class less_equal : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x <= y; }
};



// logical functor
// logical functors contains and, or, not functors.

template <typename Tp>
class logical_and : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x && y; }
};


template <typename Tp>
class logical_or : public binary_function<Tp, Tp, bool> {
public:
  bool operator()(const Tp& x, const Tp& y) const { return x || y; }
};


template <typename Tp>
class logical_not : public unary_function<Tp, bool> {
public:
  bool operator()(const Tp& x) const { return !x; }
};



// identity factor
template <typename Tp>
class identity : public unary_function<Tp, Tp> {
public:
  const Tp& operator()(const Tp& x) const { return x; }
};



// select functor
template <typename Pair>
class select1st : public unary_function<Pair, typename Pair::first_type> {
public:
  const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};


template <typename Pair>
class select2th : public unary_function<Pair, typename Pair::first_type> {
public:
  const typename Pair::second_type& operator()(const Pair& x) const { return x.second; }
};



// project functor
template <typename Arg1, typename Arg2>
class project1st : public binary_function<Arg1, Arg2, Arg1> {
public:
  Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};


template <typename Arg1, typename Arg2>
class project2st : public binary_function<Arg1, Arg2, Arg2> {
public:
  Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};

} // namespace ministl

#endif // MINISTL_FUNCTION_H