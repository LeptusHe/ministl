#include "stack.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(StackTest, construct_destroy_size_capacity_test)
{
  const int size = 100;
  ministl::stack<int> int_stack;
  ministl::stack<double> double_stack(size);
  ministl::stack<float> float_stack(size, 2.0);

  EXPECT_EQ(0, int_stack.size());
  EXPECT_EQ(size, double_stack.size());
  EXPECT_EQ(size, float_stack.size( ));
  EXPECT_TRUE(int_stack.empty( ));
  EXPECT_FALSE(double_stack.empty( ));
  EXPECT_FALSE(float_stack.empty( ));
}

TEST(StackTest, push_pop_top_test)
{
  const int size = 10;
  ministl::stack<int> int_stack;
  for (int i = 0; i < size; ++i)
    int_stack.push(i * i);
  EXPECT_EQ(size, int_stack.size( ));
  EXPECT_FALSE(int_stack.empty( ));
  EXPECT_EQ((size - 1) * (size - 1), int_stack.top( ));
  while (!int_stack.empty( ))
    int_stack.pop( );
  EXPECT_EQ(0, int_stack.size( ));
  EXPECT_TRUE(int_stack.empty( ));
}

} // namespace ministl