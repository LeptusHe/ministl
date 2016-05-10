#include "vector.h"
#include "numeric.h"
#include "functools.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(NumericAlgorithmTest, accumulate_test)
{
  const int vec_size = 100;
  ministl::vector<int> int_vec;
  EXPECT_EQ(0, accumulate(int_vec.begin( ), int_vec.end( ), 0));
  int ans = 0;
  for (int i = 0; i < vec_size; ++i) {
    int_vec.push_back(i);
    ans += i;
  }
  EXPECT_EQ(ans, accumulate(int_vec.begin( ), int_vec.end( ), 0, [](int a, int b) { return a + b; }));
}

TEST(NumericAlgorithmTest, inner_product_test)
{
  const int vec_size = 30;
  ministl::vector<int> vec1(vec_size), vec2(vec_size);
  EXPECT_EQ(0, inner_product(vec1.begin( ), vec1.end( ), vec2.begin( ), 0));
  int ans = 0;
  for (int i = 0; i < vec_size; ++i) {
    vec1[i] = i;
    vec2[i] = i + 10;
    ans += i * (i + 10);
  }
  EXPECT_EQ(ans, inner_product(vec1.begin( ), vec1.end( ), vec2.begin( ), 0,
                               [](int a, int b) { return a + b; },
                               [](int a, int b) { return a * b; }));
}

TEST(NumericAlgorithmTest, partial_sum_test)
{
  const int vec_size = 20;
  ministl::vector<int> vec1, vec2;
  EXPECT_EQ(vec2.begin( ), partial_sum(vec1.begin( ), vec1.end( ), vec2.begin( )));

  ministl::vector<int> vec3(vec_size), vec4(vec_size), result(vec_size);
  for (int i = 0, ans = 0; i < vec_size; ++i) {
    vec3[i] = i;
    ans += i;
    result[i] = ans;
  }
  partial_sum(vec3.begin( ), vec3.end( ), vec4.begin( ), [](int lhs, int rhs) { return lhs + rhs; });
  for (int i = 0; i < vec_size; ++i)
    EXPECT_EQ(result[i], vec4[i]);
}

TEST(NumericAlgorithmTest, adjacent_difference_test)
{
  const int vec_size = 20;
  ministl::vector<int> int_vec(20), result;
  for (int i = 0; i < vec_size; ++i)
    int_vec[i] = i;
  adjacent_difference(int_vec.begin( ), int_vec.end( ), std::back_inserter(result));
  EXPECT_EQ(vec_size, int_vec.size( ));
  EXPECT_EQ(0, result.front( ));
  EXPECT_TRUE(ministl::all_of(result.begin( ) + 1, result.end( ), [](int value) { return value == 1; }));
 
  adjacent_difference(int_vec.begin( ), int_vec.end( ), result.begin( ), [](int a, int b) { return a * (a - b); });
  EXPECT_TRUE(equal(int_vec.begin( ), int_vec.end( ), result.begin( )));
}

TEST(IotaAlgorithmTest, iota_test)
{
  const int start_value = 10;
  const int vec_size = 20;
  ministl::vector<int> vec(vec_size);

  iota(vec.begin( ), vec.end( ), start_value);
  for (int i = 0; i < vec.size( ); ++i)
    EXPECT_EQ(start_value + i, vec[i]);
}

} // namespace ministl