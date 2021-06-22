#pragma once

#include "Module.h"
#include "mbed.h"

namespace Module {
class WatchdogModule final : public Module {
 public:
  /* Initiates the watchdog with a countdown
   * @param countdown - max timeout of the watchdog
   * */
  WatchdogModule(std::chrono::milliseconds countdown = 5000ms);

  /* Logs the last reset due to the watchdog
   * */
  void Watchdog_logLastResetReason(void);

  /* Periodic function to kick the watchdog and restart its timer every 1s
   * */
  void periodic_1s(void) override;

 private:
  std::chrono::milliseconds countdown_ms; /*max timeout of the watchdog*/
};
}  // namespace Module