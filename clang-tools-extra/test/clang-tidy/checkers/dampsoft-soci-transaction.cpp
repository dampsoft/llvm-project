// RUN: %check_clang_tidy %s dampsoft-soci-transaction %t

namespace soci {
class transaction {};
} // namespace soci

namespace ds {
class Transaction {};
} // namespace ds

soci::transaction a;
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: soci::transaction used instead of ds::Transaction: 'a' [dampsoft-soci-transaction]
auto b = soci::transaction();
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: soci::transaction used instead of ds::Transaction: 'b' [dampsoft-soci-transaction]
soci::transaction c{};
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: soci::transaction used instead of ds::Transaction: 'c' [dampsoft-soci-transaction]

ds::Transaction d;

auto e = ds::Transaction();

ds::Transaction f();
