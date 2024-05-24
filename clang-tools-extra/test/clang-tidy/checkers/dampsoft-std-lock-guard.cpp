// RUN: %check_clang_tidy -std=c++17 %s dampsoft-std-lock-guard %t

namespace std {
template <class Mutex>
class lock_guard {
  Mutex m;

public:
  lock_guard(Mutex m) : m(m) {}
};

template <class... MutexTypes>
class scoped_lock {
public:
  scoped_lock(MutexTypes... m) {}
};

class mutex {
};
} // namespace std

std::mutex mut;
const std::lock_guard<std::mutex> lock(mut);
// CHECK-MESSAGES: :[[@LINE-1]]:35: warning: std::lock_guard used instead of std::scoped_lock: 'lock' [dampsoft-std-lock-guard]
const std::scoped_lock<std::mutex> lock2(mut);
