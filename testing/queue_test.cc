#include "queue.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(QueueTest, construct_destroy_size_capacity_test)
{
  const int size = 100;
  ministl::queue<int> int_queue;
  ministl::queue<double> double_queue(size);
  ministl::queue<float> float_queue(size, 2.0);

  EXPECT_EQ(0, int_queue.size( ));
  EXPECT_EQ(size, double_queue.size( ));
  EXPECT_EQ(size, float_queue.size( ));
  EXPECT_TRUE(int_queue.empty( ));
  EXPECT_FALSE(double_queue.empty( ));
  EXPECT_FALSE(float_queue.empty( ));
}

TEST(QueueTest, push_pop_top_test)
{
  const int size = 20;
  ministl::queue<int> int_queue;
  for (int i = 0; i < size; ++i)
    int_queue.push(i * i);
  EXPECT_EQ(size, int_queue.size( ));
  EXPECT_FALSE(int_queue.empty( ));
  EXPECT_EQ(0, int_queue.front( ));
  EXPECT_EQ((size - 1) * (size - 1), int_queue.back( ));
  int_queue.pop( );
  EXPECT_EQ(1, int_queue.front( ));
  EXPECT_EQ((size - 1) * (size - 1), int_queue.back( ));
  while (!int_queue.empty( ))
    int_queue.pop( );
  EXPECT_EQ(0, int_queue.size( ));
  EXPECT_TRUE(int_queue.empty( ));
}

} // namespace ministl