// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <gtest/gtest.h>
#include <utility>

namespace concept {

class CONCEPT : public ::testing::Test {
};

namespace min {


struct is_less_than_comparable_impl {
  template <class T, class U>
  static auto check(T*, U*) -> decltype(std::declval<T>() < std::declval<U>(), std::true_type());

  template <class T, class U>
  static auto check(...) -> std::false_type;
};

template <class T, class U>
struct is_less_than_comparable
    : decltype(is_less_than_comparable_impl::check<T, U>(nullptr, nullptr)) {};

template <class T>
const T& min(const T& a, const T& b) {
  static_assert(is_less_than_comparable<T, T>::value,
                "T must be less than comparable");

  return a < b ? a : b;
}

class X{};
}

TEST_F(CONCEPT, Min) {
  EXPECT_EQ(1, min::min(1, 2));
  // X x = min::min(X(), X());  // compile error
}

}  // namespace concept
