// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <iostream>
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

struct is_class_impl {
  template <class T>
  static std::true_type check(int T::*);

  template <class T>
  static std::false_type check(...);
};

template <class T>
struct is_class :
  public decltype(is_class_impl::check<T>(nullptr)) {};

struct X{};

TEST_F(SFINAE, IsClass) {
  static_assert(is_class<X>::value, "X is a class.");
  static_assert(!is_class<int>::value, "int is not a class.");
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

struct is_addable_impl {
  template <class T, class U>
  static auto check(T*, U*) -> decltype(
    std::declval<T>() + std::declval<U>(),
    std::true_type());

  template <class T, class U>
  static auto check(...) -> std::false_type;
};

template <class T, class U>
struct is_addable
    : decltype(is_addable_impl::check<T, U>(nullptr, nullptr)) {};

namespace addable {
struct A {};
struct B {};
    B operator+(const B&, const B&) { return B(); }
}

TEST_F(SFINAE, IsAddable) {
  static_assert(!is_addable<addable::A, addable::A>::value, "Is not addable.");
  static_assert(is_addable<addable::B, addable::B>::value, "Is addable.");
  static_assert(is_addable<int, double>::value, "Is addable.");
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

template <class Iterator,
          typename std::enable_if<
                    std::is_pointer<Iterator>::value &&
                    std::is_trivially_assignable<typename std::iterator_traits<Iterator>::value_type>::value &&
                    std::is_trivially_destructible<typename std::iterator_traits<Iterator>::value_type>::value
                   >::type* = nullptr
         >


template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last,
                    OutputIterator out) {
  std::cout << "loop copy" << std::endl;
  for (; first != last; ++out, ++first) {
    *out = *first;
  }
  return out;
}

TEST_F(SFINAE, CopyOptimization) {
  int ar1[3] = {3, 1, 4};
  int ar2[3] = {};

  sfinae::copy(std::begin(ar1), std::end(ar1), std::begin(ar2));
  for (auto i = 0; i < 3; ++i) EXPECT_EQ(ar1[i], ar2[i]);

  std::list<int> ls1 = {3, 1, 4};
  std::list<int> ls2 = {};

  sfinae::copy(std::begin(ls1), std::end(ls1), std::begin(ls2));
  EXPECT_EQ(ls1, ls2);
}

}  // namespace sfinae
