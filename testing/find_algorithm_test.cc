#include <vector>
#include <random>
#include <algorithm>

#include "gtest\gtest.h"
#include "algorithm.h"

namespace ministl{

class FindAlgorithmTest : public testing::Test {
public:
  using RandomEngine = std::default_random_engine;
public:
  static RandomEngine random_engine;
};

FindAlgorithmTest::RandomEngine FindAlgorithmTest::random_engine;

TEST_F(FindAlgorithmTest, find_test)
{
  const int TEST_TIMES = 1000;
  const int VECTOR_NUMS = 1000;
  RandomEngine num_productor;
  int need_find = num_productor();
  std::vector<int> seq(VECTOR_NUMS);
  
  for (int i = 0; i < TEST_TIMES; ++i) {
    std::generate(seq.begin( ), seq.end( ), random_engine);
    EXPECT_EQ(ministl::find(seq.begin( ), seq.end( ), need_find), std::find(seq.begin( ), seq.end( ), need_find));
  }
}

} // namespace ministl