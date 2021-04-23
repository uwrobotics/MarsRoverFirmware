
#include "WatchdogWrapper.h"
#include "mbed.h"

uint32_t countdown_ms = 1000;
uint32_t pet_ms       = 200;

int main() {
  Utility::WatchdogWrapper startWatchdog(countdown_ms, pet_ms);

  while (true)
    ;
}