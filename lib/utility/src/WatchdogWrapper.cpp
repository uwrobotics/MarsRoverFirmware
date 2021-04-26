
#include "WatchdogWrapper.h"

#include "mbed.h"

namespace Utility {

void WatchdogWrapper::startWatchdog(std::chrono::milliseconds countdown_ms /*= 5000ms*/,
                                    std::chrono::milliseconds pet_ms /*= 1000ms*/) {
  uint32_t countdown_uint32 = countdown_ms.count();
  Watchdog &watchdog        = Watchdog::get_instance();
  watchdog.start(countdown_uint32);
  Thread pet_thread;
  pet_thread.start(callback(WatchdogWrapper::petWatchdog, &pet_ms));
}

void WatchdogWrapper::petWatchdog(std::chrono::milliseconds *pet_ms) {
  while (1) {
    Watchdog::get_instance().kick();
    ThisThread::sleep_for(*pet_ms);
  }
}
}  // namespace Utility