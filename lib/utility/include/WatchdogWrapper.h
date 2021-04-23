#pragma once

namespace Utility {

class WatchdogWrapper;

class WatchdogWrapper {
 public:
  static void startWatchdog(uint32_t countdown_ms = 5000, uint32_t pet_ms = 1000);

 private:
  static void petWatchdog(uint32_t pet_ms);
};
}  // namespace Utility