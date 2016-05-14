#include "function.h"
#include "vector.h"
#include "numeric.h"
#include "algorithm.h"
#include "functools.h"
#include "gtest/gtest.h"

namespace leptus {

TEST(FunctionTest, arithmetic_test)
{
  const int vec_size = 20;
  ministl::vector<int> vec(vec_size);

  int plus_ans = 0, minus_ans = 0;
  int mul_ans = 1;
  double div_ans = 1.0;
  for (int i = 0; i < vec_size; ++i) {
    vec[i] = i + 1;
    minus_ans -= i + 1;
    plus_ans += i + 1;
    mul_ans *= i + 1;
    div_ans /= i + 1;
  }
  EXPECT_EQ(plus_ans, ministl::accumulate(vec.begin( ),
                                          vec.end( ),
                                          ministl::identity_element(ministl::plus<int>( ))));
  EXPECT_EQ(minus_ans, ministl::accumulate(vec.begin( ),
                                           vec.end( ),
                                           ministl::identity_element(ministl::plus<int>( )),
                                           ministl::minus<int>( )));
  EXPECT_EQ(mul_ans, ministl::accumulate(vec.begin( ),
                                         vec.end( ),
                                         ministl::identity_element(ministl::multiplies<int>()),
                                         ministl::multiplies<int>( )));
  EXPECT_EQ(div_ans, ministl::accumulate(vec.begin( ),
                                         vec.end( ),
                                         ministl::identity_element(ministl::multiplies<double>( )),
                                         ministl::divides<double>( )));
  EXPECT_EQ(mul_ans % plus_ans, ministl::modulus<int>( )(mul_ans, plus_ans));
  ministl::map(vec.begin( ), vec.end( ), ministl::negate<int>( ));
  EXPECT_EQ(-plus_ans, ministl::accumulate(vec.begin( ),
                                           vec.end( ),
                                           ministl::identity_element(ministl::plus<int>( ))));
}



} // namespace leptus