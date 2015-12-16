#include <vector>
#include <algorithm>
#include <functional>

#include "gtest/gtest.h"
#include "functools.h"

namespace ministl {

TEST(FunctoolsTest, MapTest)
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

} // namespace ministl