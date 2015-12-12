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
  Tp operator()(const Tp& x, const Tp& y) { return x + y; }
};


template <typename Tp>
class minus : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) { return x - y; }
};

template <typename Tp>
class multiplies : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) { return x * y; }
};

template <typename Tp>
class divides : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) { return x / y; }
};

template <typename Tp>
class modulus : public binary_function<Tp, Tp, Tp> {
public:
  Tp operator()(const Tp& x, const Tp& y) { return x % y; }
};

template <typename Tp>
class negate : public unary_function<Tp, Tp> {
public:
  Tp operator()(const Tp& x) { return -x; }
};

} // namespace ministl

#endif // MINISTL_FUNCTION_H