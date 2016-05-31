#include "heap.h"

#include <algorithm>
#include <random>
#include <vector>

#include "gtest/gtest.h"

TEST(HeapTest, push_heap_test)
{
  std::default_random_engine e;
  std::uniform_int_distribution<int> randomer(-100, 100);

  const int vec_size = 20;
  std::vector<int> vec(vec_size);
  std::generate(vec.begin( ), vec.end( ), [&]( ) { return randomer(e); });
  
  for (auto iter = vec.begin( ) + 1; iter != vec.end( ); ++iter)
    std::push_heap(vec.begin( ), iter);
  std::push_heap(vec.begin( ), vec.end());

  std::vector<int> vec_copy = vec;
  for (auto iter = vec_copy.begin( ) + 1; iter != vec_copy.end( ); ++iter)
    ministl::push_heap(vec_copy.begin( ), iter);
  ministl::push_heap(vec_copy.begin( ), vec_copy.end());

  for (int i = 0; i < vec_size; ++i)
    EXPECT_EQ(vec[i], vec_copy[i]);
}