
#include "WatchdogWrapper.h"

#include "mbed.h"

namespace Utility {

static void WatchdogWrapper::startWatchdog(uint32_t countdown_ms = 5000, uint32_t pet_ms = 1000) {
  Watchdog &watchdog = Watchdog::get_instance();
  watchdog.start(countdown_ms);
  Thread thread;
  thread.start(WatchdogWrapper::petWatchdog(pet_ms));
}

static void WatchdogWrapper::petWatchdog(uint32_t pet_ms) {
  while (1) {
    Watchdog::get_instance().kick();
    ThisThread::sleep_for(pet_ms);
  }
}
}  // namespace Utility