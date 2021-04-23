
#include "WatchdogWrapper.h"

#include "mbed.h"

namespace Utility {

void WatchdogWrapper::startWatchdog(uint32_t countdown_ms /*= 5000*/, std::chrono::milliseconds pet_ms /*= 1000ms*/) {
  Watchdog &watchdog = Watchdog::get_instance();
  watchdog.start(countdown_ms);
  Thread thread;
  thread.start(callback(&WatchdogWrapper::petWatchdog, &pet_ms));
}

void WatchdogWrapper::petWatchdog(std::chrono::milliseconds *pet_ms) {
  while (1) {
    Watchdog::get_instance().kick();
    ThisThread::sleep_for(*pet_ms);
  }
}
}  // namespace Utility