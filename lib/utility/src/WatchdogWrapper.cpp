#include "WatchdogWrapper.h"

#include "Logger.h"
#include "ResetReason.h"
#include "mbed.h"

namespace Utility {

Thread WatchdogWrapper::pet_thread;

void WatchdogWrapper::startWatchdog(std::chrono::milliseconds countdown_ms /*= 5000ms*/,
                                    std::chrono::milliseconds pet_ms /*= 1000ms*/) {
  uint32_t countdown_uint32 = countdown_ms.count();
  Watchdog &watchdog        = Watchdog::get_instance();
  watchdog.start(countdown_uint32);
  pet_thread.start(callback(WatchdogWrapper::petWatchdog, &pet_ms));
}

void WatchdogWrapper::logResetReason() {
  const reset_reason_t reason = ResetReason::get();
  if (reason == RESET_REASON_WATCHDOG) {
    time_t seconds = time(NULL);
    // Ideally we would want an accurate time and date for the timestamp, but without a reference point
    // the best we can do is the time since the microcontroller was powered on
    Utility::logger << "Reset Reason: Watchog Timer - Timestamp: " << seconds << " seconds since power on\r\n";
  }
}

void WatchdogWrapper::petWatchdog(std::chrono::milliseconds *pet_ms) {
  while (1) {
    Watchdog::get_instance().kick();
    ThisThread::sleep_for(*pet_ms);
  }
}
}  // namespace Utility