#include <vector>
#include <string>

#include "construct.h"
#include "alloc.h"
#include "gtest/gtest.h"

namespace ministl {

TEST(ConstructTest, construct_destory_test)
{
  alloc allocator;
  
  // test for construct function
  const std::string test_str = "alloc test";
  std::string* str_ptr = new std::string;
  construct(str_ptr, "alloc test");
  EXPECT_EQ(test_str, *str_ptr);

  // test for classes which have a trivial destory
  int *int_ptr = new int;
  destory(int_ptr);
  allocator.deallocate(int_ptr, sizeof(*int_ptr));

  // test for classes which have no trivial destory
  destory(str_ptr);
  allocator.deallocate(str_ptr, sizeof(*str_ptr));

  // test for range destory
  const int array_size = 10;
  int* int_array = new int[array_size];
  destory(int_array, int_array + array_size);
  allocator.deallocate(int_array, sizeof(*int_array) * array_size);

  char* char_array = new char[array_size];
  destory(char_array, char_array + array_size);
  allocator.deallocate(char_array, sizeof(*char_array) * array_size);

  std::vector<int>* vec_ptr = new std::vector<int>[array_size];
  destory(vec_ptr, vec_ptr + array_size);
  allocator.deallocate(char_array, sizeof(*vec_ptr) * array_size);
}

} // namespace ministl