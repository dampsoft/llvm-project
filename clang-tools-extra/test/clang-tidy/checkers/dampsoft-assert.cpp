// RUN: %check_clang_tidy %s dampsoft-assert %t

int abort() { return 0; }

#ifdef NDEBUG
#define assert(x) 1
#else
#define assert(x)                                                              \
  if (!(x))                                                                    \
  (void)abort()
#endif


void f() {
  assert(false);
}
// CHECK-MESSAGES: :[[@LINE-2]]:3: warning: assert() must not be used since it terminates the program! [dampsoft-assert]

void awesome_f2() {
  return;
}
