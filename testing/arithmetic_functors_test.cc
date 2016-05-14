#include <vector>
#include <iterator>
#include <algorithm>

#include "gtest/gtest.h"
#include "function.h"

namespace ministl {

class Generator {
private:
  int number_;
public:
  Generator(int start) : number_(start) { }
  int operator()( )
  {
    return number_++;
  }
};

TEST(ArithmeticFunctorsTest, plus_test)
{
  static const unsigned long long TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);
  std::vector<int> rhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));
  std::generate(lhs_numbers.rbegin( ), lhs_numbers.rend( ), Generator(INT_MIN));

  for each(auto lhs in lhs_numbers) {
    for each(auto rhs in rhs_numbers) {
      EXPECT_EQ(lhs + rhs, ministl::plus<int>( )(lhs, rhs));
    }
  }
}

TEST(ArithmeticFunctorsTest, MinusTest)
{
  static const unsigned long long TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);
  std::vector<int> rhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));
  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));

  for (auto lhs_iter = lhs_numbers.begin(); lhs_iter != lhs_numbers.end(); ++lhs_iter) {
    for (auto rhs_iter = rhs_numbers.rbegin(); rhs_iter != rhs_numbers.rend(); ++rhs_iter) {
      EXPECT_EQ(*lhs_iter - *rhs_iter, ministl::minus<int>( )(*lhs_iter, *rhs_iter));
    }
  }
}

TEST(ArithmeticFunctorsTest, MultiplesTest)
{
  static const unsigned long long TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);
  std::vector<int> rhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));
  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));

  for (auto lhs_iter = lhs_numbers.begin( ); lhs_iter != lhs_numbers.end( ); ++lhs_iter) {
    for (auto rhs_iter = rhs_numbers.rbegin( ); rhs_iter != rhs_numbers.rend( ); ++rhs_iter) {
      EXPECT_EQ(*lhs_iter * *rhs_iter, ministl::multiplies<int>( )(*lhs_iter, *rhs_iter));
    }
  }
}

TEST(ArithmeticFunctorsTest, DividesTest)
{
  static const unsigned long long TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);
  std::vector<int> rhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));
  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));

  for (auto lhs_iter = lhs_numbers.begin( ); lhs_iter != lhs_numbers.end( ); ++lhs_iter) {
    for (auto rhs_iter = rhs_numbers.rbegin( ); rhs_iter != rhs_numbers.rend( ); ++rhs_iter) {
      if (*rhs_iter == 0)
        continue;
      EXPECT_EQ(*lhs_iter * *rhs_iter, ministl::multiplies<int>( )(*lhs_iter, *rhs_iter));
    }
  }
}

TEST(ArithmeticFunctorsTest, ModulusTest)
{
  static const unsigned long long int TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);
  std::vector<int> rhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN));
  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(1000));

  for (auto lhs_iter = lhs_numbers.begin( ); lhs_iter != lhs_numbers.end( ); ++lhs_iter) {
    for (auto rhs_iter = rhs_numbers.rbegin( ); rhs_iter != rhs_numbers.rend( ); ++rhs_iter) {
      if (*rhs_iter == 0)
        continue;
      EXPECT_EQ(*lhs_iter % *rhs_iter, ministl::modulus<int>( )(*lhs_iter, *rhs_iter))
        << *lhs_iter << " " << *rhs_iter << std::endl;
    }
  }
}

TEST(ArithmeticFunctorsTest, NegateTest)
{
  static const unsigned long long int TEST_CASES_NUMBER = 1000;
  std::vector<int> lhs_numbers(TEST_CASES_NUMBER);

  std::generate(lhs_numbers.begin( ), lhs_numbers.end( ), Generator(INT_MIN + 1));

  for (auto iter = lhs_numbers.begin( ); iter != lhs_numbers.end( ); ++iter) {
    EXPECT_EQ(-*iter, ministl::negate<int>( )(*iter));
  }
}

} // namespace ministl