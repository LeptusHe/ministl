#include "deque.h"
#include "functools.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(DequeTest, construct_destory_begin_end_test)
{
  const int deque_size = 10;
  const int deque_value = 100;

  ministl::deque<std::string> str_que;
  EXPECT_EQ(0, str_que.size( ));
  EXPECT_TRUE(str_que.begin( ) == str_que.end( ));

  ministl::deque<std::string> str_que2(deque_size);
  EXPECT_EQ(deque_size, str_que2.size( ));
  EXPECT_EQ(deque_size, str_que2.end( ) - str_que2.begin( ));

  ministl::deque<int> que(deque_size, deque_value);

  EXPECT_EQ(deque_size, que.size( ));
  EXPECT_TRUE(ministl::all_of(que.begin( ), que.end( ), [deque_value](int value) { return value == deque_value; }));
}


TEST(DequeTest, front_back_test)
{
  const int deque_size = 10;
  ministl::deque<std::string> str_que(10);
  int i = 0;
  ministl::generate(str_que.begin( ), str_que.end( ), [&i]( ) { return std::to_string(i++); });
  EXPECT_EQ(std::to_string(0), str_que.front( ));
  EXPECT_EQ(std::to_string(deque_size - 1), str_que.back( ));
  str_que.push_front("front");
  EXPECT_EQ("front", str_que.front( ));
  str_que.push_back("back");
  EXPECT_EQ("back", str_que.back( ));
}


TEST(DequeTest, push_pop_front_back_test)
{
  ministl::deque<int> int_que;
  int_que.push_back(2);
  int_que.push_back(3);
  int_que.push_front(1);
  int_que.push_front(0);
  EXPECT_EQ(0, int_que.front( ));
  EXPECT_EQ(3, int_que.back( ));
  EXPECT_EQ(4, int_que.size( ));
  int_que.pop_front( );
  EXPECT_EQ(1, int_que.front( ));
  int_que.pop_back( );
  EXPECT_EQ(2, int_que.back( ));
}


TEST(DequeTest, erase_clear_test)
{
  const int deque_size = 500;
  ministl::deque<std::string> str_que(deque_size);
  for (int i = 0; i < str_que.size( ); ++i)
    *(str_que.begin( ) + i) = std::to_string(i);
  EXPECT_EQ(deque_size, str_que.size( ));
  str_que.erase(str_que.begin( ));
  EXPECT_EQ(deque_size - 1, str_que.size( ));
  EXPECT_EQ(std::to_string(1), str_que.front( ));

  str_que.clear( );
  EXPECT_EQ(0, str_que.size( ));
  EXPECT_TRUE(str_que.begin( ) == str_que.end( ));
}

TEST(DequeTest, insert_test)
{
  const int deque_size = 10, deque_value = 100;
  ministl::deque<int> int_que(deque_size);
  ministl::generate(int_que.begin( ), int_que.end( ), [deque_value]() { return deque_value; });
  for (auto i = 0; i < deque_size; ++i)
    int_que.insert(int_que.begin( ), deque_value - 1);
  for (auto i = 0; i < deque_size; ++i)
    int_que.insert(int_que.end( ), deque_value + 1);
  for (auto i = 0; i < deque_size; ++i)
    int_que.insert(int_que.begin( ) + int_que.size( ) / 2, deque_value);
  EXPECT_EQ(4 * deque_size, int_que.size( ));
  EXPECT_EQ(deque_value - 1, int_que.front( ));
  EXPECT_EQ(deque_value + 1, int_que.back( ));
}

} // namespace leptus