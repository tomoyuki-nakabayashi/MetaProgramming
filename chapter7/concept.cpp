// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <gtest/gtest.h>
#include <utility>
#include <cmath>
#include <algorithm>

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
}  // namespace min

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

class MyPoint {
    double x_ = 0;
    double y_ = 0;
 public:
    MyPoint() = default;
    MyPoint(double x, double y) : x_{x}, y_{y} {}

    double getX() const { return x_; }
    double getY() const { return y_; }
};

template <class T>
struct point_traits {
  static double x(const T& p) { return p.x(); }
  static double y(const T& p) { return p.y(); }

  static T subtract(const T& a, const T& b) {
    return T(a.x() - b.x(), a.y() - b.y());
  }
};

template <>
struct point_traits<MyPoint> {
  static double x(const MyPoint& p) { return p.getX(); }
  static double y(const MyPoint& p) { return p.getY(); }

  static MyPoint subtract(const MyPoint& a, const MyPoint&b) {
    return MyPoint(a.getX() - b.getX(), a.getY() - b.getY());
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
  MyPoint mpa(0.0, 0.0);
  MyPoint mpb(3.0, 3.0);

  EXPECT_NEAR(4.24264, distance(a, b), 0.00001);
  EXPECT_NEAR(4.24264, distance(mpa, mpb), 0.00001);
}

template <class T>
struct line_segment_traits {
  typedef typename T::point_type point_type;

  static const point_type& p1(const T& line) { return line.p1(); }
  static const point_type& p2(const T& line) { return line.p2(); }
};

struct point_category {};
struct line_segment_category {};

template <class T>
struct get_geometry_category {};

template <>
struct get_geometry_category<point> {
  typedef point_category type;
};

template <class Point>
class line_segment {
  Point p1_;
  Point p2_;
 public:
  typedef Point point_type;

  line_segment() = default;
  line_segment(const Point& p1, const Point& p2): p1_{p1}, p2_{p2} {}

  const Point& p1() const { return p1_; }
  const Point& p2() const { return p2_; }
};

template <class Point>
struct get_geometry_category<line_segment<Point>> {
  typedef line_segment_category type;
};

template <class Point>
double distance_impl(Point a, Point b, point_category, point_category) {
  typedef point_traits<Point> traits;
  const Point d = traits::subtract(a, b);
  return std::sqrt(traits::x(d) * traits::x(d) + traits::y(d) * traits::y(d));
}

template <class Point, class LineSegment>
double distance_impl(Point a, LineSegment b, point_category, line_segment_category) {
  typedef line_segment_traits<LineSegment> traits;
  return std::min(distance(a, traits::p1(b)), distance(a, traits::p2));
}

template <class LineSegment, class Point>
double distance_impl(LineSegment a, Point b, line_segment_category, point_category) {
  return distance(b, a);
}

template <class Geometry1, class Geometry2>
double distance(Geometry1 a, Geometry2 b) {
  return distance_impl(a, b,
      typename get_geometry_category<Geometry1>::type(),
      typename get_geometry_category<Geometry2>::type());
}

TEST_F(CONCEPT, OverloadedDistance) {
  point p1(0.0, 0.0);
  point p2(3.0, 3.0);
  auto line = line_segment<point>(point(2.0, 2.0), point(3.0, 3.0));

  EXPECT_NEAR(4.24264, distance(p1, p2), 0.00001);
  //EXPECT_NEAR(4.24264, distance(p1, line), 0.00001);
}

}  // namespace concept
