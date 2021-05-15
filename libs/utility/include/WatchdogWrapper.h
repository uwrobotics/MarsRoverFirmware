#pragma once
#include "mbed.h"

namespace Utility {

class WatchdogWrapper {
 public:
  static void startWatchdog(std::chrono::milliseconds countdown_ms = 5000ms, std::chrono::milliseconds pet_ms = 1000ms);
  static void logResetReason();

 private:
  static void petWatchdog(std::chrono::milliseconds *pet_ms);
  static Thread pet_thread;
};
}  // namespace Utility