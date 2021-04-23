#pragma once

namespace Utility {

class WatchdogWrapper;

class WatchdogWrapper {
 public:
  static void startWatchdog(uint32_t countdown_ms = 5000, std::chrono::milliseconds pet_ms = 1000ms);

 private:
  static void petWatchdog(std::chrono::milliseconds *pet_ms);
};
}  // namespace Utility