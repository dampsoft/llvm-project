// RUN: %check_clang_tidy %s dampsoft-assert %t

#include <assert.h>

void f() {
  assert(false);
}
// CHECK-MESSAGES: :6:3: warning: assert() must not be used since it terminates the program! [dampsoft-assert]

void awesome_f2() {
  return;
}
