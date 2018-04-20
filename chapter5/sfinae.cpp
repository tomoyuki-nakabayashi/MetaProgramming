// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <gtest/gtest.h>

namespace sfinae {

class SFINAE : public ::testing::Test {
};

struct has_iterator_impl {
  template <class T>
  static std::true_type check(typename T::iterator*);

  template <class T>
  static std::false_type check(...);
};

template <class T>
class has_iterator : public decltype(has_iterator_impl::check<T>(nullptr)) {};

TEST_F(SFINAE, HasIterator) {
  static_assert(has_iterator<std::vector<int>>::value, "vector has iterator.");
  static_assert(!has_iterator<int>::value, "int does not have iterator.");
}

struct is_assignable_impl {
  template <class T>
  static auto check(T*)
      -> decltype(std::declval<T&>() = std::declval<const T&>(),
                  std::true_type());

  template <class T>
  static auto check(...) -> std::false_type;
};

template <class T>
struct is_assignable : decltype(is_assignable_impl::check<T>(nullptr)) {};

struct A {
  A& operator=(const A&) = delete;
};

struct B {};

TEST_F(SFINAE, IsAssignable) {
  static_assert(!is_assignable<A>::value, "Cannot substitute.");
  static_assert( is_assignable<B>::value, "Can substitute.");
}

struct has_sort_member_impl {
  template <class T>
  static auto check(T*) -> decltype(
    std::declval<T>().sort(),
    std::true_type()
  );

  template <class T>
  static auto check(...) -> std::false_type;
};

template <class T>
struct has_sort_member :
  decltype(has_sort_member_impl::check<T>(nullptr)) {};

template <class Sortable,
  typename std::enable_if<has_sort_member<Sortable>::value>::type* = nullptr>
void sort(Sortable &x) {
  x.sort();
}

template <class Range,
  typename std::enable_if<!has_sort_member<Range>::value>::type* = nullptr>
void sort(Range& r) {
  std::sort(std::begin(r), std::end(r));
}

TEST_F(SFINAE, SwitchSort) {
  std::vector<int> v = {3, 1, 4};
  std::list<int> ls = {3, 1, 4};

  sort(v);
  sort(ls);

  std::vector<int> expect_v = {1, 3, 4};
  std::list<int> expect_ls = {1, 3, 4};
  EXPECT_EQ(expect_v, v);
  EXPECT_EQ(expect_ls, ls);
}



}  // namespace sfinae
