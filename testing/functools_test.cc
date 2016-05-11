#include <algorithm>
#include <functional>
#include <string>

#include "vector.h"
#include "functools.h"

#include "gtest/gtest.h"

namespace ministl {

TEST(FunctoolsTest, map_test)
{
  int data = 0;
  std::function<int(int)> square = [] (int n)->int { return n * n; };
  std::vector<int> test_data(1000);
  std::generate(test_data.begin( ), test_data.end( ), [&] ( ){ return data++; });

  std::vector<int> excepted_data = test_data;
  ministl::map(test_data.begin( ), test_data.end( ), square);

  auto pred = [] (int x, int y) { return x == y * y; };
  auto mismatch_point = std::mismatch(test_data.begin( ), test_data.end( ), excepted_data.begin( ), pred);
  auto excepted_result = std::make_pair<decltype(test_data.end( )), decltype(excepted_data.end( ))>(test_data.end( ), excepted_data.end( ));
  EXPECT_EQ(excepted_result, mismatch_point) << *mismatch_point.first << " " << *mismatch_point.second << std::endl;
}

TEST(FunctoolsTest, for_each_test)
{
  const int vec_size = 10;
  ministl::vector<int> int_vec(vec_size);
  int start_value = 0, ans = 0;
  ministl::generate(int_vec.begin( ), int_vec.end( ),
                    [&start_value, &ans]( ) { ans += start_value; return start_value++; });
  int test_ans = 0;
  ministl::for_each(int_vec.begin( ), int_vec.end( ), [&test_ans](int x) { test_ans += x; });
  EXPECT_EQ(ans, test_ans);
}

} // namespace ministl