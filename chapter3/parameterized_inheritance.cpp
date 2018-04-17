#include <iostream>
#include <cstdlib>

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

int main() {
  Bold<Plain> b;
  std::string s1 = b.convert("Hello");
  std::cout << s1 << std::endl;
  
  Italic<Plain> i;
  std::string s2 = i.convert("Hello");
  std::cout << s2 << std::endl;
  
  Italic<Bold<Plain>> ib;
  std::string s3 = ib.convert("Hello");
  std::cout << s3 << std::endl;

  return 0;
}