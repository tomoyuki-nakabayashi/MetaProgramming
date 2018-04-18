// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <cstdint>
#include <gtest/gtest.h>

namespace meta_function {

template <int32_t N>
struct increment {
  static const int value = N + 1;
};

template <int32_t N>
struct fractorial {
  static const int value = N * fractorial<N - 1>::value;
};

template <>
struct fractorial<0> {
  static const int value = 1;
};

class MetaFunction : public ::testing::Test {
};

TEST_F(MetaFunction, Increment) {
  int32_t n = increment<3>::value;
  EXPECT_EQ(4, n);
}

TEST_F(MetaFunction, Fractorial) {
  int32_t result = fractorial<3>::value;
  EXPECT_EQ(6, result);
}

template <int N>
struct int_{};

template <typename T, int N,
  typename std::enable_if<(N > 0)>::type* = nullptr>
auto add_pointer(T, int_<N>)
  -> decltype(add_pointer(std::declval<T*>(), int_<N-1>()));

template <typename T, int N,
  typename std::enable_if<N == 0>::type* = nullptr>
auto add_pointer(T, int_<N>) -> T;

typedef decltype(add_pointer(std::declval<int>(), int_<3>())) result;
}  // namespace meta_function
