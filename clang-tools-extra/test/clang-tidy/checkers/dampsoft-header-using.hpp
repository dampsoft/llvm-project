// RUN: %check_clang_tidy %s dampsoft-header-using %t

namespace ds {
class Test {};
} // namespace ds

using namespace ds;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: 'using namespace' mustn't be called in headers! [dampsoft-header-using]

ds::Test a;
