#pragma once
#include "mbed.h"

namespace Utility {

class WatchdogWrapper {
 public:
  static void startWatchdog(std::chrono::milliseconds countdown_ms);
  static void logResetReason(void);
  static void petWatchdog(void);
};
}  // namespace Utility