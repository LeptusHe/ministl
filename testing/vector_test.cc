#include "vector.h"
#include "algorithm.h"
#include <algorithm>
#include "gtest/gtest.h"

namespace ministl {

TEST(VectorTest, construct_destory_test)
{
  ministl::vector<int> vec_size(10);
  EXPECT_EQ(10, vec_size.size( ));
  EXPECT_FALSE(vec_size.empty( ));

  ministl::vector<std::string> vec_string;
  EXPECT_EQ(nullptr, vec_string.begin( ));
  EXPECT_EQ(nullptr, vec_string.end( ));
  EXPECT_EQ(0, vec_string.size());
  EXPECT_EQ(0, vec_string.capacity());
  EXPECT_TRUE(vec_string.empty( ));

  ministl::vector<std::string> vec_strs(10, "test");
  EXPECT_TRUE(std::all_of(vec_strs.begin( ), vec_strs.end( ), [](const std::string& str) { return str == "test"; }));
}

TEST(VectorTest, begin_end_front_back_test)
{
  ministl::vector<int> vec(10);
  for (int i = 0; i < vec.size( ); ++i)
    vec[i] = i;
  EXPECT_EQ(0, vec.front( ));
  EXPECT_EQ(9, vec.back( ));
  EXPECT_EQ(0, *vec.begin());
  EXPECT_EQ(9, *(vec.end( ) - 1));
}

TEST(VectorTest, push_back_pop_back_test)
{
  const int test_cases = 100;
  const int test_value = 10;
  ministl::vector<int> vec(test_cases);
  std::generate(vec.begin( ), vec.end( ), [test_value]( ) { return test_value; });
  EXPECT_EQ(test_cases, vec.size( ));
  EXPECT_TRUE(std::all_of(vec.begin( ), vec.end( ), [test_value](int value) { return value == test_value; }));
  for (int i = 0; i < test_cases; ++i)
    vec.pop_back( );
  EXPECT_EQ(0, vec.size( ));
  EXPECT_EQ(test_cases, vec.capacity( ));
}

TEST(VectorTest, insert_erase_test)
{
  const int test_value = 5;
  const int test_cases = 20;
  
  // insert test
  ministl::vector<int> vec;
  // insert in start
  vec.insert(vec.begin(), test_cases, test_value);
  EXPECT_EQ(test_cases, vec.size( ));
  EXPECT_TRUE(std::all_of(vec.begin( ), vec.end( ), [test_value](int value) { return value == test_value; }));
  // insert in end
  vec.insert(vec.end( ), test_cases, test_value * test_value);
  EXPECT_EQ(test_cases * 2, vec.size( ));
  EXPECT_TRUE(std::all_of(vec.begin() + vec.size() / 2, vec.end(),
                          [test_value](int value) { return value == test_value * test_value; }));

  // erase test
  vec.erase(vec.begin( ) + vec.size( ) / 2, vec.end( ));
  EXPECT_EQ(test_value, vec.back( ));
  EXPECT_EQ(test_cases, vec.size( ));
  vec.insert(vec.begin( ), 1, 0);
  vec.erase(vec.begin( ));
  EXPECT_EQ(test_value, vec.front( ));
  vec.push_back(test_value + 1);
  vec.push_back(test_value + 2);
  vec.erase(vec.end( ) - 1);
  EXPECT_EQ(test_value + 1, vec.back( ));
}

} // namespace ministl