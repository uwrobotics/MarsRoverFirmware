
#include "WatchdogWrapper.h"
#include "mbed.h"

std::chrono::milliseconds countdown_ms = 1000ms;
std::chrono::milliseconds pet_ms       = 200ms;

int main() {
  Utility::WatchdogWrapper::startWatchdog(countdown_ms, pet_ms);

  MBED_ASSERT(false);
}