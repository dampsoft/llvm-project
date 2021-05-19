// RUN: %check_clang_tidy %s dampsoft-inline-static-variable %t

class Test {
public:
  Test(char b) {}
};

inline static Test Foo{'b'};
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: static inline variable definition detected: 'Foo' [dampsoft-inline-static-variable

inline static Test func() {}
