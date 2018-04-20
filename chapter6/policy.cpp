// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the Apache 2.0 License, see LICENSE.

#include <iostream>
#include <string>
#include <gtest/gtest.h>


namespace policy {

class POLICY : public ::testing::Test {
};

struct debug_log {
  static void print(const std::string& value) {
    std::cout << value << std::endl;
  }
};

struct release_log {
  static void print(const std::string&) {}
};

template <class LogPolicy>
struct hoge {
  void foo() const {
    LogPolicy::print("Fatal Error""");
  }
};

template <class Strategy>
struct ownership {
  typedef Strategy type;
};

struct reference_count {};
struct deep_copy {};

template <bool IsMultiThread>
struct multi_thread {
  static const bool value = IsMultiThread;
};

template <class>
struct is_ownership_policy;

template <class>
struct is_multi_thread_policy;

template <class... Args>
struct smart_ptr {
  typedef typename get_required_arg<is_ownerchip_policy, Args...>::type
      ownership_policy;

  typedef typename get_optional_arg<multi_thread<false>, is_multi_thread_policy,
                                    Args...>::type multi_thread_policy;
}

}  // namespace policy
