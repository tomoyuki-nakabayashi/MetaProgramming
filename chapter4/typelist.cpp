// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <cstdint>
#include <gtest/gtest.h>

namespace meta_function {

template <class Head, class... Tail>
struct g {
  typedef Head head;
  typedef std::tuple<Tail...> tail;
};

template <class... List>
struct f {
  typedef typename g<List...>::head head;
  typedef typename g<List...>::tail tail;
};

template <template <class> class Pred, class Head, class... Tail>
struct any_of {
  static const bool value = Pred<Head>::value ? true : any_of<Pred, Tail...>::value;
};

template <template <class> class Pred, class Head>
struct any_of<Pred, Head> {
  static const bool value = Pred<Head>::value;
};

template <class T>
struct is_int {
  static const bool value = false;
};

template <>
struct is_int<int> {
  static const bool value = true;
};

class TypeList : public ::testing::Test {
};

TEST_F(TypeList, FindInt) {
  const bool result = any_of<is_int, char, double, int>::value;
  EXPECT_TRUE(result);
}

}  // namespace meta_function
