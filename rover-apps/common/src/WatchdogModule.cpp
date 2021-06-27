#include "WatchdogModule.h"

#include "Module.h"
#include "WatchdogWrapper.h"
#include "mbed.h"

const std::chrono::milliseconds WatchdogModule::WATCHDOG_DEFAULT_COUNTDOWN = 5000ms;

WatchdogModule::WatchdogModule(){
  Utility::WatchdogWrapper::logResetReason();
  Utility::WatchdogWrapper::startWatchdog(WATCHDOG_DEFAULT_COUNTDOWN);
}

void WatchdogModule::periodic_1s(void) {
  Utility::WatchdogWrapper::petWatchdog();
}
