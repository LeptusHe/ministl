#include "alloc.h"
#include "gtest/gtest.h"

namespace ministl {

// test cases for template malloc_alloc_template
TEST(MallocAllocTemplateTest, allocate_deallocate_reallocate_test)
{
  malloc_alloc allocator;
  const int constant_value = 10;
  int* int_value = (int*)allocator.allocate(sizeof(*int_value));
  *int_value = constant_value;
  EXPECT_EQ(constant_value, *int_value); // test for allocate
  
  // test for reallocate
  const double constant_double = 3.141592654;
  double* double_value = (double*)allocator.reallocate(int_value, sizeof(*int_value), sizeof(double));
  *double_value = constant_double;
  EXPECT_EQ(constant_double, *double_value);

  // test for deallocate
  allocator.deallocate(double_value, sizeof(double));
}

// test cases for default_alloc_template
TEST(DefaultAllocTemplateTEST, allocate_deallocate_reallocate_test)
{
  alloc allocator;
  const int constant_value = 10;
  int* int_value = (int*)allocator.allocate(sizeof(*int_value));
  *int_value = constant_value;
  EXPECT_EQ(constant_value, *int_value); // test for allocate

                                         // test for reallocate
  const double constant_double = 3.141592654;
  double* double_value = (double*)allocator.reallocate(int_value, sizeof(*int_value), sizeof(double));
  *double_value = constant_double;
  EXPECT_EQ(constant_double, *double_value);

  // test for deallocate
  allocator.deallocate(double_value, sizeof(double));
}

} // namespace ministl