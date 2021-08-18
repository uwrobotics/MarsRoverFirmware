#include "WatchdogWrapper.h"
#include "mbed.h"

Thread pet_thread;
std::chrono::milliseconds countdown_ms = 1000ms;
std::chrono::milliseconds pet_ms       = 200ms;

void pet_dog_task(std::chrono::milliseconds *pet_ms) {
  while (1) {
    Utility::WatchdogWrapper::petWatchdog();
    ThisThread::sleep_for(*pet_ms);
  }
}

int main() {
  Utility::WatchdogWrapper::logResetReason();
  Utility::WatchdogWrapper::startWatchdog(countdown_ms);
  pet_thread.start(callback(pet_dog_task, &pet_ms));
  ThisThread::sleep_for(2000ms);
  MBED_ASSERT(false);
}
