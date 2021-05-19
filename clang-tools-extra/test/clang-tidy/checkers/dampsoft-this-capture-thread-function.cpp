// RUN: %check_clang_tidy %s dampsoft-this-capture-thread-function %t

namespace ds {
template <class ResultType>
class ThreadFunction {

public:
  template <class F>
  explicit ThreadFunction(F f) {
  }
};
} // namespace ds

class Test {
  void test() {
    ds::ThreadFunction<void>([this]() -> void {});
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: ThreadFunction mustn't capture 'this'! [dampsoft-this-capture-thread-function]
    auto b = 5;
    ds::ThreadFunction<void>([b]() -> void {});
  }

  void someFunc() {
  }
};
