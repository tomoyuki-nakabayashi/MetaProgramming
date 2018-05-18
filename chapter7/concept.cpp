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

class point {
    double x_ = 0;
    double y_ = 0;
 public:
    point() = default;
    point(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }
};

point operator-(const point& a, const point& b) {
  return point(a.x() - b.x(), a.y() - b.y());
}

template <class T>
struct point_traits {
  static double x(const T& p) { return p.x(); }
  static double y(const T& p) { return p.y(); }

  static T subtract(const T& a, const T& b) {
    return T(a.x() - b.x(), a.y() - b.y());
  }
};

template <class Point>
double distance(Point a, Point b) {
  typedef point_traits<Point> traits;
  const Point d = traits::subtract(a, b);
  return std::sqrt(traits::x(d) * traits::x(d) + traits::y(d) * traits::y(d));
}

TEST_F(CONCEPT, Point) {
  point a(0.0, 0.0);
  point b(3.0, 3.0);

  EXPECT_NEAR(4.24264, distance(a, b), 0.00001);
}

}  // namespace concept
