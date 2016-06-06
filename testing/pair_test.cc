#include <random>
#include <algorithm>
#include <functional>

#include "gtest\gtest.h"
#include "pair.h"

namespace ministl {

TEST(PairTest, pair_test)
{
  const int TEST_CASES = 1000;
  std::default_random_engine rand_engine;
  std::uniform_real_distribution<double> double_dist(INT_MIN, INT_MAX);

  std::vector<int> int_vector(TEST_CASES);
  std::vector<double> double_vector(TEST_CASES);
  std::vector<ministl::pair<int, double>> pairs(TEST_CASES);

  std::generate(int_vector.begin( ), int_vector.end( ), rand_engine);
  std::generate(double_vector.begin( ), double_vector.end( ), [&]( ) { return double_dist(rand_engine); });
  std::transform(int_vector.begin( ), int_vector.end( ), double_vector.begin( ), pairs.begin(),
                 std::function<ministl::pair<int, double>(const int&, const double&)>(ministl::make_pair<int, double>));

  for (auto i = 0; i < TEST_CASES; ++i) {
    EXPECT_EQ(pairs[i].first, int_vector[i]);
    EXPECT_EQ(pairs[i].second, double_vector[i]);
    
    auto pair_actual = ministl::make_pair<int, double>(int_vector[i], double_vector[i]);
    EXPECT_TRUE(pairs[i] == pair_actual);
  }
  
}

} // namespace ministl