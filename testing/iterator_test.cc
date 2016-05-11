#include "algorithm.h"
#include "functools.h"
#include "iterator.h"
#include "deque.h"
#include "vector.h"

#include <string>
#include <iterator>
#include <algorithm>

#include "gtest/gtest.h"

namespace ministl {

TEST(IteratorTest, front_inserter_test)
{
  const int que_value = 10, que_size = 10;
  int test_value = 10;
  ministl::deque<int> int_que(que_size), result_que;
  for (int i = 0; i < que_size; ++i)
    int_que[i] = i;
  ministl::copy(int_que.begin( ), int_que.end( ), ministl::front_inserter(result_que));
  EXPECT_TRUE(ministl::all_of(ministl::make_reverse_iterator(result_que.end( )),
                              ministl::make_reverse_iterator(result_que.begin( )),
                              [test_value](int value) mutable { return value == --test_value; }));
}

TEST(IteratorTest, back_inserter_test)
{
  const int que_value = 10, que_size = 20;
  int start_value = 0;
  ministl::deque<int> int_que(que_size), result_que;
  ministl::generate(int_que.begin( ), int_que.end( ), [start_value]( ) mutable { return start_value++; });
  ministl::copy(int_que.begin( ), int_que.end( ), ministl::back_inserter(result_que));
  EXPECT_TRUE(ministl::equal(int_que.begin( ), int_que.end( ), result_que.begin( )));
}

TEST(IteratorTest, insert_iterator_test)
{
  const int que_value = 10, que_size = 20;
  int start_value = 0;
  ministl::deque<int> int_que(que_size), result_que;
  ministl::generate(int_que.begin( ), int_que.end( ), [start_value]( ) mutable { return start_value++; });
  ministl::copy(int_que.begin( ) + 1, int_que.end( ), ministl::inserter(result_que, result_que.begin()));
  EXPECT_EQ(que_size - 1, result_que.size( ));
  EXPECT_TRUE(ministl::equal(int_que.begin( ) + 1, int_que.end( ), result_que.begin( )));
}


TEST(IteratorTest, istream_iterator_test)
{
  const int deque_size = 20;
  std::string input_text;
  for (int i = 0; i < deque_size; ++i)
    input_text += std::to_string(i) + " ";
  std::istringstream istrstream(input_text);

  ministl::istream_iterator<std::string> str_iterator(istrstream);
  ministl::istream_iterator<std::string> eos;
  ministl::deque<std::string> str_que;
  ministl::copy(str_iterator, eos, std::back_inserter(str_que));
  int start_value = 0;
  EXPECT_TRUE(ministl::all_of(str_que.begin( ), str_que.end( ),
                              [start_value](const std::string& value) mutable {
    return std::to_string(start_value++) == value; }));
}


TEST(IteratorTest, ostream_iterator_test)
{
  const int que_size = 20;
  int start_value = 10;
  std::ostringstream string_stream;
  ministl::deque<std::string> str_que(que_size);
  ministl::generate(str_que.begin( ), str_que.end( ),
                    [start_value]( ) mutable { return std::to_string(start_value++); });
  ministl::copy(str_que.begin( ), str_que.end( ),
                ministl::ostream_iterator<std::string>(string_stream, " "));
  std::string ans;
  for (auto& str : str_que)
    ans += str + " ";
  EXPECT_EQ(ans, string_stream.str( ));
}

TEST(IteratorTest, advance_func_test)
{
  const int vec_size = 10;
  int start_value = 0;
  ministl::vector<int> int_vec(vec_size);
  ministl::generate(int_vec.begin( ), int_vec.end( ), [start_value]( ) mutable { return start_value++; });
  for (auto iter = int_vec.begin( ); iter != int_vec.end( ); advance(iter, 1)) {
    EXPECT_EQ(start_value++, *iter);
  }

  auto start_iter = ministl::make_reverse_iterator(int_vec.end( ));
  auto end_iter = ministl::make_reverse_iterator(int_vec.begin( ));
  for (auto iter = start_iter; iter != end_iter; advance(iter, 1))
    EXPECT_EQ(--start_value, *iter);
}


TEST(IteratorTest, distance_func_test)
{
  const int vec_size = 10;
  ministl::vector<int> int_vec(vec_size);
  EXPECT_EQ(vec_size, ministl::distance(int_vec.begin( ), int_vec.end( )));
}

} // namespace ministl