#include "queue.h"
#include "vector.h"
#include "gtest/gtest.h"

#include "queue.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(PriorityQueueTest, construct_destroy_size_test)
{
  const int size = 50;
  auto comp = [](int lhs, int rhs) { return rhs > lhs; };
  ministl::priority_queue<int> int_pri_queue1;
  ministl::priority_queue<int, ministl::vector<int>, decltype(comp)> int_pri_queue2(comp);
  
  ministl::vector<int> vec(size);
  ministl::priority_queue<int> int_pri_queue3(vec.begin( ), vec.end( ));
  ministl::priority_queue<int, ministl::vector<int>, decltype(comp)> int_pri_queue4(vec.begin( ), vec.end( ), comp);

  EXPECT_EQ(0, int_pri_queue1.size( ));
  EXPECT_EQ(0, int_pri_queue2.size( ));
  EXPECT_EQ(size, int_pri_queue3.size( ));
  EXPECT_EQ(size, int_pri_queue4.size( ));
  EXPECT_TRUE(int_pri_queue1.empty( ));
  EXPECT_TRUE(int_pri_queue2.empty( ));
  EXPECT_FALSE(int_pri_queue3.empty( ));
  EXPECT_FALSE(int_pri_queue4.empty( ));
}

TEST(PrioryQueueTest, top_test)
{
  const int size = 20;
  ministl::vector<int> int_vec;
  ministl::priority_queue<int> int_priority_queue;
  for (int i = 0; i < size; ++i) {
    int_priority_queue.push(i);
    int_vec.push_back(i);
  }
  ministl::priority_queue<int> int_priority_queue_copy(int_vec.begin( ), int_vec.end( ));
  EXPECT_EQ(int_priority_queue_copy.size( ), int_priority_queue.size( ));
  EXPECT_EQ(int_priority_queue.top( ), int_priority_queue_copy.top( ));
}

TEST(PrioryQueueTest, push_pop_test)
{
  const int size = 20;
  ministl::vector<int> int_vec;
  ministl::priority_queue<int> int_priority_queue;
  for (int i = 0; i < size; ++i) {
    int_vec.push_back(i);
    int_priority_queue.push(i);
  }

  ministl::priority_queue<int> int_priority_queue_copy(int_vec.begin( ), int_vec.end( ));
  EXPECT_EQ(int_priority_queue_copy.size( ), int_priority_queue.size( ));
  for (int i = 0; i < int_priority_queue.size( ); ++i) {
    EXPECT_EQ(int_priority_queue.top( ), int_priority_queue_copy.top( ));
    int_priority_queue.pop( );
    int_priority_queue_copy.pop( );
  }
}

} // namespace ministl