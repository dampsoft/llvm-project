// RUN: %check_clang_tidy %s dampsoft-lambda-capture-leak %t

class Test {
public:
  template <typename L>
  void addListener(L lambda) {
    lambda();
  }

  void doNothing() const {}
};

void test() {
  auto TestInstance = Test();
  TestInstance.addListener([TestInstance]() { TestInstance.doNothing(); });
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: Lambda leak (self scoped SharedPtr) found [dampsoft-lambda-capture-leak]

  auto TestInstance2 = Test();
  TestInstance.addListener([TestInstance2]() { TestInstance2.doNothing(); });

  TestInstance.addListener([&TestInstance]() { TestInstance.doNothing(); });
}
