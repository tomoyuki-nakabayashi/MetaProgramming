// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <cstdint>
#include <string>
#include <sstream>
#include <gtest/gtest.h>

namespace basic_inheritance {
class Plain {
 public:
    std::string convert(const std::string& str) const {
      return str;
    }
};

template <typename Base>
class Bold : public Base {
 public:
    std::string convert(const std::string& str) const {
      return "<b>" + Base::convert(str) + "</b>";
    }
};

template <typename Base>
class Italic : public Base {
 public:
    std::string convert(const std::string& str) const {
      return "<i>" + Base::convert(str) + "</i>";
    }
};

class BasicInheritance : public ::testing::Test {
};

TEST_F(BasicInheritance, Test) {
  Bold<Plain> b;
  std::string s1 = b.convert("Hello");
  EXPECT_EQ("<b>Hello</b>", s1);

  Italic<Plain> i;
  std::string s2 = i.convert("Hello");
  EXPECT_EQ("<i>Hello</i>", s2);
  
  Italic<Bold<Plain>> ib;
  std::string s3 = ib.convert("Hello");
  EXPECT_EQ("<i><b>Hello</b></i>", s3);
}

}  // namespace basic_inheritance

namespace virtualized_inheritance {
class Plain {
 public:
    virtual std::string convert(const std::string& str) const {
      return str;
    }
};

template <typename Base = Plain>
class Bold : public Base {
 public:
    using Base::Base;  // Inherit constructor.

    std::string convert(const std::string &str) const override {
      return "<b>" + Base::convert(str) + "</b>";
    }
};

template <typename Base = Plain>
class Italic : public Base {
 public:
    using Base::Base;  // Inherit constructor.

    virtual std::string convert(const std::string &str) const override {
      return "<i>" + Base::convert(str) + "</i>";
    }
};

template <typename Base = Plain>
class Size : public Base {
 public:
    using Base::Base;  // Inherit constructor.
    Size(int32_t s) {
      std::ostringstream ostrm;
      ostrm << s;
      size_ = ostrm.str();
    }

    virtual std::string convert(const std::string &str) const override {
      return "<font size='" + size_ + "'>" + Base::convert(str) + "</font>";
    }

 private:
    std::string size_;
};

class VirtualizedInheritance : public ::testing::Test {
};

TEST_F(VirtualizedInheritance, Test) {
  Plain p;
  Bold<> b;
  Italic<> i;
  Size<Bold<Italic<>>> sbi(5);
  Italic<Size<>> is(3);

  EXPECT_EQ("Hello", p.convert("Hello"));
  EXPECT_EQ("<b>Hello</b>", b.convert("Hello"));
  EXPECT_EQ("<i>Hello</i>", i.convert("Hello"));
  EXPECT_EQ("<font size='5'><b><i>Hello</i></b></font>", sbi.convert("Hello"));
  EXPECT_EQ("<i><font size='3'>Hello</font></i>", is.convert("Hello"));
}

}  // namespace virtualized_inheritance
