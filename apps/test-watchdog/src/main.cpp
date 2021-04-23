
#include "WatchdogWrapper.h"
#include "mbed.h"

uint32_t countdown_ms            = 1000;
std::chrono::milliseconds pet_ms = 200ms;

int main() {
  Utility::WatchdogWrapper::startWatchdog(countdown_ms, pet_ms);

  while (true)
    ;
}