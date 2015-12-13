#ifndef MINISTL_FUNCTION_H
#define MINISTL_FUNCTION_H

//namespace ministl {

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



// functor adapters: not1, not2

// not1 functor adapter
template <class Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
  Predicate pred_;
public:
  explicit unary_negate(const Predicate& pred) : pred_(pred) { }

  bool operator()(const typename Predicate::argument_type& x) const { return !pred_(x) }
};

template <typename Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred)
{
  return unary_negate<Predicate>(pred);
}


// not2 functor adapter
template <class Predicate>
class binary_negate : public binary_function<typename Predicate::first_argument_type,
                                            typename Predicate::second_argument_type,
                                            bool> {
protected:
  Predicate pred_;
public:
  explicit unary_negate(const Predicate& pred) : pred_(pred) { }

  bool operator()(const typename Predicate::first_argument_type& x,
                  const typename Predicate::second_argument_type& y) const { return !pred_(x, y); }
};

template <typename Predicate>
inline unary_negate<Predicate> not2(const Predicate& pred)
{
  return binary_negate<Predicate>(pred);
}


// bind1st functor adapter
template <typename Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
                                         typename Operation::result_type> {
protected:
  Operation op_;
  typename Operation::first_argument_type value_;
public:
  explicit binder1st(const Operation& x, const typename Operator::first_argument_type& y)
    : op_(x), value_(y) { }

  typename Operation::result_type operator()(const typename Operator::second_argument_type& y) const
  {
    return op_(value_, y);
  }
};

template <typename Operation, typename Tp>
inline binder1st<Operation> bind1st(const Operation& fn, const Tp& x)
{
  typedef typename Operation::first_argument_type Arg1_type;
  return binder1st<Operation>(fn, Arg1_type(x));
}


// bind2st
template <typename Operation>
class binder2st : public unary_function<typename Operation::first_argument_type,
                                        typename Operation::result_type> {
protected:
  Operation op_;
  typename Operation::second_argument_type value_;
public:
  explicit binder2st(const Operation& op, typename Operation::second_argument_type& x)
    :op_(op), value_(x) { }

  typename Operation::result_type operator()(const Operation& op,
                                             const typename Operation::first_argument_type& x) const
  {
    return op_(x, value_);
  }
}

template <typename Operation, class Tp>
inline binder2nd<Operation>
bind2st(const Operation& fn, const Tp& x)
{
  typedef Operation::second_argument_type Arg2_type;
  return bind2st<Operation>(fn, Arg2_type(x));
}


// compose1 functor adapter
template <typename Operation1, typename Operation2>
class unary_compose : public unary_function<typename Operation2::argument_type,
                                            typename Operation1::result_type> {
protected:
  Operation1 fn1_;
  Operation2 fn2_;
public:
  explicit unary_compose(const Operation1& fn1, const Operation2& fn2) const
    : fn1_(fn1), fn2_(fn2) { }

  typename Operation1::result_type operator(typename Operation2::argument_type x) const
  {
    return fn1_(fn2_(x));
  }
};

template <typename Operation1, typename Operation2>
inline unary_compose<Operation1, Operation2> compose1(const Operation1& fn1, const Operation2& fn2)
{
  return unary_compose<Operation1, Operation2>(fn1, fn2);
}



// compose2 functor adapter
template <typename Operation1, typename Operation2, typename Operation3>
class binary_compose : public unary_function<typename Operation2::argument_type,
                                             typename Operation1::result_type> {
protected:
  Operation1 fn1_;
  Operation2 fn2_;
  Operation3 fn3_;
public:
  explicit binary_compose(const Operation1& x, const Operation2& y, const Operation3& z)
    : fn1_(x), fn2_(y), fn3_(z) { }

  typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const 
  {
    return fn1_(fn2_(x), fn3_(x));
  }
};

template <typename Operation1, typename Operation2, typename Operation3>
inline binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1& fn1, const Operation2& fn2,
                                                                   const Operation3& fn3, )
{
  return binary_compose<Operation1, Operation2, Operation3>(fn1, fn2, fn3);
}


// pointer_to_unary_function factor adapter


//} // namespace ministl

#endif // MINISTL_FUNCTION_H