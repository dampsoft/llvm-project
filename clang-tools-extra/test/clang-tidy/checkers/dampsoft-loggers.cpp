// RUN: %check_clang_tidy %s dampsoft-loggers %t

namespace ds {
namespace log {
class Logger {};
} // namespace log
} // namespace ds

ds::log::Logger someNameLogger;

static ds::log::Logger someOtherLogger;

static ds::log::Logger logger;
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: invalid (too generic) name for ds::log::Logger 'logger' [dampsoft-loggers]
// CHECK-FIXES: {{^}}static ds::log::Logger nonGenericName;{{$}}

static ds::log::Logger nonGenericNamedLogger;
